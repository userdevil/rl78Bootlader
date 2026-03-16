
#pragma NOP

#include "r_cg_macrodriver.h"
#include "command.h"
#include "r_cg_userdefine.h"
#include "fsl.h"
#include "fsl_types.h"
#include "r_cg_serial.h"
#include "r_cg_timer.h"
#include "r_cg_wdt.h"

		

extern unsigned char rx_data;	
extern unsigned char GetByte ( unsigned long timeout );
extern unsigned char GetDelayTimerStatus (void);
extern void SendString (unsigned  char *str );
extern volatile __boolean DelayTimerUnderFlowFlag;
void Blank_Check_Error(unsigned char );
void RunCommandHandler (void);
extern unsigned char XmodemDownloadAndProgramFlash (unsigned long FlashAddress);
extern fsl_u08 g_xm_last_fsl_status;
unsigned char first_user_flash_address[]="0002000";
unsigned char str1[]="RL78/G13 64P 64K Bootloader Main Menu Ver-2";
unsigned char str2[]="-------------------------------------------";
unsigned char str3[]="1......Blank Check User Area";
unsigned char str4[]="2......Erase User Area";
//unsigned char str5[]="3......Erase User Block";
unsigned char str6[]="3......Program Flash via XModem Download";
unsigned char str7[]="4......Start User Program";
unsigned char str8[]=">";
	
unsigned char c1_str1[]="Blank checking user area...";
unsigned char c1_str2[]="Block: ";
unsigned char c1_str3[]=": NOT blank";
unsigned char c1_str4[]=": BLANK";

unsigned char c2_str1[]="Really erase ALL user blocks (Y/N)?";
unsigned char c2_str2[]="Timed out waiting for response";
unsigned char c2_str3[]="Comms error";
unsigned char c2_str4[]="All blocks erase cancelled";
unsigned char c2_str5[]="Erasing block ";
unsigned char c2_str6[]=" >> ";
unsigned char c2_str7[]=": Erasing success!!";
unsigned char c2_str8[]="Erasing of block: ";
unsigned char c2_str9[]=": FAILED";
unsigned char c2_str10[]="All user blocks erased";

unsigned char c3_str1[]="Enter block no to be erased as 2-digit number 16-254, e.g. 09";
unsigned char c3_str2[]=">D'";
unsigned char c3_str3[]="Timed out waiting for block number...";
unsigned char c3_str4[]="Invalid block number";
unsigned char c3_str5[]="Erase block (Y/N)?";
unsigned char c3_str6[]="Timed out waiting for confirmation";
unsigned char c3_str7[]="Erasing of block ";
unsigned char c3_str8[]="PASSED ";
unsigned char c3_str9[]=" FAILED ";
unsigned char c3_str10[]="Block erase cancelled";

unsigned char c4_str1[]="Download to address 0x";
unsigned char c4_str2[]=" (Y/N)?";
unsigned char c4_str3[]="Timed out waiting for confirmation...";
unsigned char c4_str4[]="Program Flash (Y/N)?";
unsigned char c4_str5[]="Timed out waiting for response";
unsigned char c4_str6[]="Comms error";
unsigned char c4_str7[]="Flash programming cancelled";
unsigned char c4_str8[]="Start XModem download...";
unsigned char c4_str9[]="Xmodem Executing...";
unsigned char c4_str10[]="Xmodem Execution end!!!";
unsigned char c4_str11[]="Timeout";
unsigned char c4_str12[]="Download OK";
unsigned char c4_str13[]="Flash program FAIL FSL=0x";
unsigned char c4_str14[]="NOT WORKING";

unsigned char c5_str1[]="Run user program (Y/N)?";
unsigned char c5_str2[]="Timed out waiting for response";
unsigned char c5_str3[]="Comms error";
unsigned char c5_str4[]="Run user program request cancelled";
unsigned char c5_str5[]="Running user program...";
unsigned char c5_str6[]="Invalid user reset address.  Staying put. No valid user program";

//
// functionality of this handler will be dependent on the communication hardware interface
//
/**************************************/
/*    RunCommandHandler function      */
/**************************************/

void RunCommandHandler (void)
{
	__far unsigned int *ptr;
	pt2FunctionErase fp;		// function pointer used to call target program
	unsigned char rx_status;
	// check if bootloader should run by detecting activity for ~2 seconds on the
	// UART channel (200 ticks × ~10 ms/tick = ~2 s)
	rx_status = GetByte (200);
	
	if ( rx_status == TIMEOUT )
	{
		R_WDT_Restart();

		// timed out - call target app if reset vector is valid
		ptr = (__far unsigned int *)( FIRST_USER_FLASH_ADDR );
		if ( *ptr != 0xffff )
		{
			// Stop bootloader peripherals and disable interrupts for a clean handoff
			R_TAU0_Channel0_Stop();
			DI();
			R_WDT_Restart();
			// call user program and never return
			fp = (pt2FunctionErase) ptr;
			fp( 0, 0);
		}
		else
		{
			// return back to main
			return;
		}
	}
	else
	{
		// serial activity detected
		// clear buffer
		// loop until we get a timeout
		//PurgeComms( 50 );	
		// display menu
 		ShowMenu();
			
		while (1)
		{
			// timeout is not important as we know we have a byte waiting for us
			rx_status = GetByte (100000);
			if ( rx_status == OK )
			{
				R_WDT_Restart();
				switch ( rx_data )
				{
					case '1':
						Command_1();		// blank check
					break;
					
					case '2':
						Command_2();		// erase all user flash blocks
					break;
					
//					case '3':
//						Command_3();		// erase specific flash block
//					break;
					
					case '3':
						Command_4();		// xmodem download to flash memory
					break;

					case '4':
						Command_5();		// run target application
						
					break;
					default:
						ShowMenu();
				}
			}
		}
	}
}

/**************************************/
/*    ShowMenu function               */
/**************************************/
void ShowMenu (void)
{

	SendLFCR();
	SendLFCR();
	SendString(str1);//"RL78G1x Bootloader Main Menu v1.0" );
	SendLFCR();
	SendString(str2);// "----------------------------------"  );
	SendLFCR();
	SendLFCR();
	SendString(str3);// "1......Blank Check User Area" );
	SendLFCR();
	SendLFCR();
	SendString(str4);// "2......Erase User Area" );
	SendLFCR();
	SendLFCR();
//	SendString(str5);// "3......Erase User Block" );
//	SendLFCR();
//	SendLFCR();
	SendString(str6);// "4......Program Flash via XModem Download" );
	SendLFCR();
	SendLFCR();
	SendString(str7);// "5......Start User Program" );
	SendLFCR();
	SendLFCR();
	SendString(str8);// '>' );
}

/**************************************/
/*    Command_1 function              */
/**************************************/
void Command_1 (void)
{
	// blank check the user flash area
	// the user flash area is specified as the range of addresses between
	// 'FIRST_USER_FLASH_ADDR' and 'LAST_USER_FLASH_ADDR' inclusive
	// these values are specified in 'command.h'

	
	read_datum *address;
	unsigned char blk,tmp;
	fsl_u08 my_fsl_status;
	
	SendLFCR();
	SendString(c1_str1);// "Blank checking user area..." );
	
	FSL_Open();    	
	FSL_PrepareFunctions();	
	
	for(blk=Start_Block;blk<=No_Of_Blocks;blk++)
	{
		SendLFCR();
		SendString(c1_str2);// "Block: " );
		if(blk < 9){
			SendByte(blk+48);
		}else{
			tmp = blk / 100;
			SendByte(tmp+48);
			tmp = blk % 100;
			tmp = tmp / 10;
			SendByte(tmp+48);
			tmp = blk % 10;
			SendByte(tmp+48);
		}
		my_fsl_status = FSL_BlankCheck(blk);			//Blank Check if Device is not blank issue Erase Command
		if(my_fsl_status != FSL_OK){
			SendString(c1_str3);// ": NOT blank" ); 
			//Blank_Check_Error(blk);
			MyErrorHandler();
		}else{
			NOP();	
			SendString(c1_str4);// ": BLANK" );
		}
	}
	
	SendLFCR();	
	FSL_Close();
	
}

/**************************************/
/*    Command_2 function              */
/**************************************/
void Command_2 (void)
{
	// erase all user flash blocks
	// the first flash block to be erased is 'FIRST_USER_FLASH_BLK' and the last
	// flash block to be erased is 'NO_OF_FLASH_BLOCKS' - 1
	// these values are defined in 'command.h' and 'flash_header.h'	
	
	unsigned char uc,tmp1,tmp2;
	unsigned char Status;
	unsigned char rxstatus;
	fsl_u08 my_fsl_status;
	
	
	SendLFCR();
	
	SendString(c2_str1);// "Really erase ALL user blocks (Y/N)?" );
	rxstatus = GetByte( 10000 );
	SendByte( rx_data );
	if ( rxstatus == TIMEOUT )
	{
		SendLFCR();
		SendString(c2_str2);// "Timed out waiting for response" );
		return;
	}
	else if ( rxstatus == ERROR )
	{
		SendLFCR();
		SendString(c2_str3);// "Comms error" );
		return;
	}
	else if ( !( ( rx_data == 'y' ) || ( rx_data == 'Y' ) ) )
	{
		SendLFCR();
		SendString(c2_str4);// "All blocks erase cancelled" );
		return;
	}
	R_WDT_Restart();
	FSL_Open();
	R_WDT_Restart();
	FSL_PrepareFunctions();
	for( uc=Start_Block; uc<=No_Of_Blocks; uc++ )
	{
		R_WDT_Restart();
		SendLFCR();
		SendString(c2_str5);// "Erasing block " );
		if(uc < 9){
			SendByte(uc+48);
		}else{
			
			tmp1 = uc / 100;
			SendByte(tmp1+48);
			tmp1 = uc % 100;
			
			tmp1 = tmp1 / 10;
			SendByte(tmp1+48);
			tmp2 = uc % 10;
			SendByte(tmp2+48);
		}		
		SendString (c2_str6);// " >> " );
		// call the erase function with the block to be erased and a dummy value (0)
		
		my_fsl_status = FSL_Erase(uc);
		while(my_fsl_status == FSL_BUSY)
		{
			R_WDT_Restart();  /* erase takes up to ~20ms per block — feed WDT */
			my_fsl_status = FSL_StatusCheck();
		}
				
		
		if(my_fsl_status == FSL_OK){
			R_WDT_Restart();
			SendString(c2_str7);//": Erasing success!!");


		}else if(my_fsl_status != FSL_OK){	
			SendString(c2_str8);// "Erasing of block: " );
			SendString(c2_str9);// ": FAILED" );
			SendLFCR();
			MyErrorHandler();
			return;
		}
		
		
	}
	FSL_Close();
	SendLFCR();
	SendString(c2_str10);// "All user blocks erased" );
}

/**************************************/
/*    Command_3 function              */
/**************************************/
//void Command_3 (void)
//{
//	// erase the specified flash block
//	unsigned char status,tmp,tmpblknm[2];	
//	int tmpblk,BlockNum, Count;
//	fsl_u08 my_fsl_status;
//
//	BlockNum = 0;
//	
//	SendLFCR();
//	SendString(c3_str1);// "Enter block no to be erased as 2-digit number e.g. 08" );
//	SendLFCR();
//	SendString(c3_str2);// ">D'" );
//	
//	for(tmp=0;tmp<2;tmp++){
//		status = GetByte( 10000 );		
//		tmpblknm[tmp] = rx_data;	
//		SendByte( tmpblknm[tmp] );
//		if ( status == TIMEOUT )
//		{
//			SendLFCR();
//			SendString(c3_str3);// "Timed out waiting for block number..." );
//			return;
//		}else
//			tmpblk = tmpblknm[0] - 48; 
//	}
//	tmpblk = tmpblk*10;
//	tmpblk = tmpblk+(tmpblknm[1] - 48);
//	//SendByte( (tmpblk/10)+48 );
//	//SendByte( (tmpblk%10)+48 );
//	BlockNum = tmpblk;
//	if ( BlockNum > No_Of_Blocks)
//	{
//		SendLFCR();
//		SendString(c3_str4);// "Invalid block number" );
//		return;
//	}
//	
//	SendLFCR();
//	SendString(c3_str5);// "Erase block (Y/N)?" );
//	status = GetByte( 10000 );
//	if(status == OK)
//		SendByte( rx_data);
//	
//	if ( status == TIMEOUT )
//	{
//		SendLFCR();
//		SendString(c3_str6);// "Timed out waiting for confirmation" );
//		return;
//	}
//	else if (  ( rx_data== 'y' ) || ( rx_data == 'Y' ) )
//	{
//		// call the erase function with the block to be erased and a dummy value (0)
//		//Count = BlockErase ( BlockNum, 0 );
//		FSL_Open();    	
//		FSL_PrepareFunctions();	
//		my_fsl_status = FSL_Erase(BlockNum);
//		while(my_fsl_status == FSL_BUSY)
//		{
//		my_fsl_status = FSL_StatusCheck();
//		}
//		if(my_fsl_status != FSL_OK)
//		MyErrorHandler();
//	
//		SendLFCR();
//		SendString(c3_str7);// "Erasing of block " );
//		SendByte( (BlockNum/10) + 48 );
//		SendByte( (BlockNum%10) + 48 );
//		if ( my_fsl_status == FSL_OK )
//		{
//			SendString(c3_str8);// " PASSED " );
//		} 
//		else
//		{
//			SendString(c3_str9);// " FAILED " );
//		}
//		FSL_Close();
//		return;
//	}
//	else
//	{
//		SendLFCR();
//		SendString(c3_str10);// "Block erase cancelled" );
//		FSL_Close();
//		return;
//	}
//}

/**************************************/
/*    Command_4 function              */
/**************************************/
void Command_4 (void)
{
    // download data using xmodem and program into flash
    unsigned char Status;
	unsigned long Address;

	// No Y/N confirmations — selecting option 3 from the menu is already
	// confirmation. Prompting here causes a deadlock: TeraTerm's XMODEM Send
	// dialog waits for NAK while the bootloader waits for keyboard Y/N input.
	Address = FIRST_USER_FLASH_ADDR;

	SendLFCR();
	SendString(c4_str8);// "Start XModem download..." );
	SendLFCR();
	SendString(c4_str9);// "Xmodem Executing..." );
	R_WDT_Restart();
	// start xmodem download and program the flash with the downloaded data
	Status = XmodemDownloadAndProgramFlash( Address );
	SendLFCR();
	SendString(c4_str10);// "Xmodem Execution end!!!" );	
	if ( Status == XM_TIMEOUT )
		SendString(c4_str11);// "Timeout" );
	else if ( Status == XM_OK ){
		SendString(c4_str12);// "Download OK" );
			
	}
	else if ( Status == XM_PROG_FAIL )
	{
		unsigned char nibble;
		SendString(c4_str13);  /* "Flash program FAIL FSL=0x" */
		nibble = (g_xm_last_fsl_status >> 4) & 0x0FU;
		SendByte((unsigned char)(nibble < 10U ? nibble + '0' : nibble - 10U + 'A'));
		nibble = g_xm_last_fsl_status & 0x0FU;
		SendByte((unsigned char)(nibble < 10U ? nibble + '0' : nibble - 10U + 'A'));
	}
	else SendString(c4_str14);//"NOT WORKING");

	SendLFCR();
	/* FSL_Close is already called inside XmodemDownloadAndProgramFlash on all
	 * exit paths — do NOT call it again here. */
}

/**************************************/
/*    Command_5 function              */
/**************************************/
void Command_5 (void)
{
	// call the target application
	unsigned char rxstatus;
	__far unsigned int *ptr;
	pt2FunctionErase fp;
	


	
	SendLFCR();
	SendString(c5_str1);// "Run user program (Y/N)?" );
	rxstatus = GetByte( 10000 );
	SendByte( rx_data );
	if ( rxstatus == TIMEOUT )
	{
		SendLFCR();
		SendString(c5_str2);// "Timed out waiting for response" );
		return;
	}
	else if ( rxstatus == ERROR )
	{
		SendLFCR();
		SendString(c5_str3);// "Comms error" );
		return;
	}
	else if ( !( ( rx_data == 'y' ) || ( rx_data == 'Y' ) ) )
	{
		SendLFCR();
		SendString(c5_str4);// "Run user program request cancelled" );
		return;
	}
	
	SendLFCR();
	
	// run user program
	// check user reset address is not all erased
	ptr = (__far unsigned int *)( FIRST_USER_FLASH_ADDR );

	if ( *ptr != 0xffff )
	{
		SendString(c5_str5);// "Running user program..." );
		// Stop bootloader peripherals and disable interrupts for a clean handoff
		R_TAU0_Channel0_Stop();
		DI();
		R_WDT_Restart();
		// call user program and never return
		fp = (pt2FunctionErase) ptr;
		fp( 0, 0);
	}
	else
	{
		// invalid user reset address
		SendString(c5_str6);// "Invalid user reset address.  Staying put." );
		return;
	}
}