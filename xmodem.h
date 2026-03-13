/****************************************************************************************/
/*                                                                     					*/
/*  M16C/62P Group Program Collection                                  					*/
/*                                                                     					*/
/*  FILE NAME : xmodem.h                                       							*/
/*  CPU       : This program is the Serial flash boot loader by Xmodem data transfer	*/
/*  HISTORY   : 2006.12.08 Ver 0.01                                    					*/
/*                                                                    	 				*/
/*  Copyright (C) 2006. Renesas Technology Corp.                       					*/
/*  Copyright (C) 2006. Renesas Solutions Corp.                        					*/
/*  All right reserved.                                                					*/
/*                                                                     					*/
/*****************************************************************************************/

//#include "serial.h"

#ifndef _XMODEM_H
#define _XMODEM_H

#define SOH					0x01
#define EOT					0x04
#define ACK					0x06
#define	NAK					0x15
#define CAN					0x18

#define XM_OK					0x00
#define XM_ADDRESS_ERROR			0x01
#define XM_COMMS_ERROR				0x02
#define XM_TIMEOUT				0x04
#define XM_PROG_FAIL				0x08

// function prototypes


#endif
