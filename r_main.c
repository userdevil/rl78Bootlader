/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2013 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : bl_main.c
* Version      : CodeGenerator for RL78/G13 V2.01.00.01 [26 Jul 2013]
* Device(s)    : R5F100LJ
* Tool-Chain   : CA78K0R
* Description  : This file implements main function.
* Creation Date: 03-01-2014
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_serial.h"
#include "r_cg_timer.h"
/* Start user code for include. Do not edit comment generated here */
#include "fsl.h"
#include "fsl_types.h"
#include "command.h"
#include "bl_ram_vector.h"
#include"string.h"
#include "r_cg_wdt.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
unsigned int i;
__boolean rx_flag;
__boolean tx_flag;
unsigned char rec_val;
unsigned char intro_str[]="RL78/G13 UART (9.6 Kbps) BOOT LOADER Programming...";
unsigned char tmpr;
unsigned char rx_data;	
unsigned long tick_count;
unsigned char err_flag;
unsigned char status;
extern __boolean DelayTimerUnderFlowFlag;
  
unsigned char Data_Clear[9] = {27, 91, 50, 74, 0, 27, 91, 72, 0}; 
unsigned char my_data[8] = "BL F S L";
unsigned char ReadByte(void);
void SendByte (unsigned char);
unsigned char RxByteWaiting(void);
unsigned char TxByteEmpty (void);
unsigned char GetByte (unsigned long);
unsigned char GetDelayTimerStatus (void);
unsigned char PurgeComms (unsigned long);
void SendString (unsigned  char *str );
unsigned char xbit[5];
/* End user code. Do not edit comment generated here */
void BL_MAIN_UserInit(void);

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    /* Start user code. Do not edit comment generated here */
    //BL_Systeminit();
    __near fsl_write_t bl_fsl_write_str;
   
   	unsigned char *fsl_data_buffer_u08;
    	fsl_u08 bl_fsl_status;
	fsl_descriptor_t fsl_descr;
	unsigned char us_c;
	
	BL_MAIN_UserInit();
	
		//SendString(intro_str);
		for(us_c=0;us_c<9;us_c++)
		{
			R_WDT_Restart();
			SendByte(Data_Clear[us_c]);	
		}
		
		SendLFCR();
		
		SendString(intro_str);
		
		fsl_descr.fsl_flash_voltage_u08 = 0x00;
		fsl_descr.fsl_frequency_u08 = 0x14;
		fsl_descr.fsl_auto_status_check_u08 = 0x01;
	
		bl_fsl_status = FSL_Init((__far fsl_descriptor_t*)&fsl_descr);
		if(bl_fsl_status != FSL_OK) 
		MyErrorHandler();
		R_WDT_Restart();
			
		SendLFCR();
		R_WDT_Restart();
		RunCommandHandler();

    /* If RunCommandHandler returns (no user app and no serial activity),
     * keep looping so the bootloader can still accept programming commands.
     * WDT is fed inside GetByte() which is called on each iteration. */
    while(1)
    {
    	R_WDT_Restart();
    	RunCommandHandler();
    }

    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void BL_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
		EI();
		R_SAU0_Create();
		R_UART0_Start();
		R_TAU0_Create();
		
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

void MyErrorHandler(void)
{
	NOP();
}
unsigned char ReadByte(void)
{
	while(SRIF0 == 0) { R_WDT_Restart(); }	//Wait until a byte is received, feeding WDT
	SRIF0 = 0;
	return (RXD0);
}
void SendByte ( unsigned char aChar )
{
	STMK0 = 1U;			/* disable INTST0 interrupt */
	/* Do NOT clear SRIF0 here — clearing the RX flag inside a TX function
	 * discards any byte that arrived just before or during transmission,
	 * which causes the first byte of the next incoming XMODEM packet to be
	 * lost and produces continuous NAK retries (~3 Bytes/s). */
	TXD0	= aChar;
	while(STIF0 == 0);			//Wait until buffer becomes empty
	STIF0 = 0;
	NOP();

	STMK0 = 0U;    /* enable INTST0 interrupt */
}
unsigned char RxByteWaiting(void)
{
R_WDT_Restart();				
	if(SRIF0 == 1){
		tmpr = 1;
		NOP();
	}else {
		
		tmpr = 0;	
		NOP();
	}
	
        NOP();
	return(tmpr);
	
}
unsigned char TxByteEmpty ( void )
{
	//
	// returns 1 - if the transmit register is empty
	// returns 0 - if the transmit register is not empty
	//
	unsigned char tmp;	
	if( STIF0 == 0 )
	{
		tmp = 0;
	}
	else
	{
		tmp = 1;
	}
	
	return tmp;
}

unsigned char GetByte ( unsigned long timeout )
{
	R_WDT_Restart();
	
	//	timeout = time in ms for which to timeout the data reception
	//
	//	returns a 16-bit value
	//	upper (msb) byte contains status info
	//	lower (lsb) byte contains the data received from the SCI channel
	//
	//	status:
	//	OK
	//	ERROR
	//	TIMEOUT
	//
	
	
	
	tick_count = (unsigned long) timeout / MS_PER_TIMER_COMPARE_MATCH;
	/* Do NOT clear SRIF0 here — a byte that arrived since the last GetByte
	 * call (e.g. during flash programming) must not be discarded. */
	R_TAU0_Channel0_Start();

	// wait for a byte to arrive
	while ( ( RxByteWaiting() == 0 ) && (tick_count) )
	{
		R_WDT_Restart();
		if ( GetDelayTimerStatus() )
		{
			/* Timer underflow detected inside the loop body — this is a race:
			 * tick_count hit 0 between the while-condition check and here.
			 * Stop the timer and do NOT restart it; the while condition will
			 * see tick_count==0 and exit on the next iteration.
			 * (Old code restarted the timer with tick_count==0, causing the
			 * next ISR to wrap tick_count to 0xFFFFFFFF → infinite spin.) */
			R_TAU0_Channel0_Stop();
		}
	}

	if ( RxByteWaiting() == 1 )
	{
		SRIF0 = 0;			/* clear flag now that we are about to read */
		DelayTimerUnderFlowFlag = 0;
		R_TAU0_Channel0_Stop();
	// read error flags and data
		err_flag = SSR01L;
		// check for errors
		// Rx error
		if((err_flag>=1)&&(err_flag<=7)){	//Error
			/* Clear sticky UART error flags (OVF/FEF/PEF) so they do not
			 * poison every subsequent GetByte call. */
			SIR01 = (unsigned short)(err_flag & 0x07U);
			status = ERROR;
		}
		else
		{
			// no Rx error
			rx_data = RXD0;
			status = OK;
		}
	}
	else
	{
		DelayTimerUnderFlowFlag = 0;
		R_TAU0_Channel0_Stop();		/* stop timer — was left running after tick_count reached 0 */
		status = TIMEOUT;
	}
	
	return (status);
}

unsigned char GetDelayTimerStatus (void)
{
	if ( DelayTimerUnderFlowFlag == 1 )
	{
		NOP();
		return (1);
		
	}			   
	else
	{
		NOP();
		return (0);
	}
}
unsigned char PurgeComms(unsigned long timeout)
{
	unsigned char rxstatus;

	/* Drain any pending bytes from UART until no new byte is received within
	 * the timeout window. Returning after the first GetByte() leaves stale
	 * bytes in the FIFO, which can misalign the first XMODEM frame. */
	do
	{
		rxstatus = GetByte(timeout);
	} while (rxstatus == OK);

	return rxstatus;
}

void SendLFCR(void)
{
	SendByte(LINEFEED);
	SendByte(CARRIAGE_RETURN);
}

void SendString(unsigned char *str)
{
	unsigned short us;

	us = 0;
	while ( str[us] != 0 )
	{
		SendByte ( (unsigned char) str[us] );
		us++;
	}
}

/* End user code. Do not edit comment generated here */
