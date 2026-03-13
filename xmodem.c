
#include <string.h>
#include "r_cg_userdefine.h"
#include "fsl.h"
#include "fsl_types.h"
#include "r_cg_wdt.h"
volatile union {
		unsigned char uc[ 128 + 4 + 1 ];
		read_datum ur[ (128 + 4 + 1) / sizeof (read_datum) ];
	} RxByteBuffer;
unsigned char XmodemDownloadAndProgramFlash (unsigned long FlashAddress);
extern unsigned char rx_data;
extern unsigned char PurgeComms ( unsigned long timeout );
extern unsigned char GetByte ( unsigned long timeout );
extern unsigned char GetDelayTimerStatus (void);
/***********************************************/
/*    XmodemDownloadAndProgramFlash function   */
/***********************************************/
unsigned char XmodemDownloadAndProgramFlash (unsigned long FlashAddress)
{

/*
XmodemDownloadAndProgramFlash() takes a memory address as the base address to
which data downloaded is programmed.  The data is downloaded using the XModem
protocol developed in 1982 by Ward Christensen.
The routine detects errors due to timeouts, comms errors or invalid checksums.
The routine reports the following to the caller:
-Success
-Invalid address
-Comms error
-Timeout error
-Failure to program flash


Expects:
--------
FlashAddress:
32-bit address located in Flash memory space starting on a 32-byte boundary

Returns:
--------
XM_OK			-	Download and Flash programming performed ok
XM_ADDRESS_ERROR	-	Address was either not on a 128-bit boundary or not in valid Flash
XM_COMMS_ERROR		-	Comms parity, framing or overrun error
XM_TIMEOUT		-	Transmitter did not respond to this receiver
XM_PROG_FAIL		-	Failed to program one or more bytes of the Flash memory
*/
	unsigned char rxstatus;
	unsigned char ExpectedBlkNum;
	unsigned char RetryCounter;
	unsigned char RxByteCount;
	unsigned char RxByteBufferIndex;
	unsigned char Status;
	unsigned char checksum;
	unsigned char StartCondition;
	unsigned long Address;
	fsl_u08 my_fsl_status;
	fsl_write_t my_fsl_write_str;

	// To ensure that data is stored starting on the correct size boundary for the Flash
	// an extra byte is needed as this buffer must store the xmodem protocol bytes
	// and the data bytes.  This means that a padding byte is added at the beginning.

	// first xmodem block number is 1
	ExpectedBlkNum = 1;

	StartCondition = 0;
	Address = FlashAddress;
	R_WDT_Restart();

	// Open FSL once for the entire transfer — NOT per block.
	// Calling FSL_Open/PrepareFunctions for every packet added ~ms of overhead,
	// during which Tera Term was already sending the next packet, causing UART
	// overrun and continuous NAK retries (~3 Bytes/s instead of ~960 Bytes/s).
	FSL_Open();
	FSL_PrepareFunctions();

	// Flush any stale bytes already in the UART Rx buffer
	rxstatus = PurgeComms(1);

	while(1)
	{
		//	{1}
		//	initialise Rx attempts
		RetryCounter = 10;

		// decrement Rx attempts counter & get Rx byte; repeat until Rx attempts is 0
		// Each attempt allows ~1 s (100 ticks x ~10 ms/tick); 10 retries = ~10 s total
		// (XModem standard: receiver sends NAK every 10 s and waits up to 60 s)
		rxstatus = TIMEOUT;
		while ( (RetryCounter > 0) && (rxstatus == TIMEOUT) )
		{
			R_WDT_Restart();
			if (StartCondition == 0)
			{
				//	if this is the start of the xmodem frame
				//	send a NAK to the transmitter
				SendByte( NAK );
				rxstatus = GetByte( 100 );
			}
			else
			{
				rxstatus = GetByte( 100 );
			}
			RetryCounter--;
		}

		StartCondition = 1;

		//	if timed out after 10 attempts or comms error
		//	return relevant error state to caller
		if ( rxstatus == TIMEOUT )
		{
			FSL_Close();
			return ( XM_TIMEOUT );
		}
		else if ( rxstatus == ERROR )
		{
			FSL_Close();
			return ( XM_COMMS_ERROR );
		}
		else
		{
			// if first received byte is 'end of frame'
			// return ACK to sender and exit
			if ( rx_data == EOT )
			{
				SendByte( ACK );
				FSL_Close();
				return( XM_OK );
			}
			else
			{
				//	initialise counter for incoming Rx bytes
				// start of header + block num + (255 - block num) + 128 data bytes + checksum
				RxByteCount = 128 + 4;
				// RxByteBufferIndex is initialised to 1 to ensure correct boundary for the data
				RxByteBufferIndex = 1;

				rxstatus = XM_OK;

				// store the byte we have just received
				RxByteBuffer.uc[ RxByteBufferIndex++ ] = rx_data;
				RxByteCount--;

				while( RxByteCount > 0 )
				{
					R_WDT_Restart();
					//	get Rx byte with 1 second timeout
					rxstatus = GetByte( 100 );

					//	if timed out or comms error
					if ( (rxstatus == TIMEOUT) || (rxstatus == ERROR) )
					{
						rxstatus = XM_TIMEOUT;
						// send NAK and loop back to start of while loop
						SendByte( NAK );
						RxByteCount = 0;
					}
					else
					{
						// no timeout or comms error
						// store Rx byte
						RxByteBuffer.uc[ RxByteBufferIndex++ ] = rx_data;
						RxByteCount--;
					}
				}

				if (rxstatus == XM_TIMEOUT)
				{
					// loop back to (1) - do nothing
				}
				else
				{
					// data Rx ok
					// calculate the checksum of the data bytes only
					checksum = 0;
					for (RxByteBufferIndex=0; RxByteBufferIndex<128; RxByteBufferIndex++)
					{
						checksum += RxByteBuffer.uc[ RxByteBufferIndex + 3 + 1 ];
					}

					//	if SOH, BLK#, 255-BLK# or checksum not valid
					//	(BLK# is valid if the same as expected blk counter or is 1 less)
					if ( !( (RxByteBuffer.uc[0 + 1] == SOH) &&
					        ((RxByteBuffer.uc[1 + 1] == ExpectedBlkNum) || (RxByteBuffer.uc[1 + 1] == ExpectedBlkNum - 1)) &&
					        (RxByteBuffer.uc[2 + 1] + RxByteBuffer.uc[1 + 1] == 255) &&
					        (RxByteBuffer.uc[131 + 1] == checksum) ) )
					{
						//	send NAK and loop back to (1)
						SendByte( NAK );
					}
					else
					{
						//	if blk# is expected blk num
						if ( RxByteBuffer.uc[1 + 1] == ExpectedBlkNum )
						{
							// Program the received 128-byte block into flash.
							// FSL is already open (opened once before the transfer loop).
							my_fsl_write_str.fsl_data_buffer_p_u08 = (fsl_u08 *)&RxByteBuffer.uc[3 + 1];
							my_fsl_write_str.fsl_word_count_u08 = 0x20; /* 32 words = 128 bytes */
							my_fsl_write_str.fsl_destination_address_u32 = Address;
							my_fsl_status = FSL_Write((__near fsl_write_t*)&my_fsl_write_str);

							if(my_fsl_status == FSL_OK)
							{
								Status = PROG_PASS;
							}
							else
							{
								MyErrorHandler();
								Status = PROG_FAIL;
							}

							if( Status == PROG_PASS )
							{
								//	if prog routine passed ok increment flash address by 128
								Address += 128;
								ExpectedBlkNum++;
								SendByte( ACK );
								//	loop back to (1)
							}
							else
							{
								// prog fail — abort transfer
								SendByte( NAK );
								SendByte( CAN );
								FSL_Close();
								return( XM_PROG_FAIL );
							}
						}
						else
						{
							//	block number is valid but this data block has already been received
							//	send ACK and loop to (1)
							SendByte( ACK );
						}
					}
				}
				R_WDT_Restart();
			}
			R_WDT_Restart();
		}
		R_WDT_Restart();
	}

	return(XM_OK);		//	Processing doesn't come here.
					//	The purpose is to erase the Warning output of the compiler.

}
