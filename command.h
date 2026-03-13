/****************************************************************************************/
/*                                                                     					*/
/*  M16C/62P Group Program Collection                                  					*/
/*                                                                     					*/
/*  FILE NAME : command.h                                       						*/
/*  CPU       : This program is the Serial flash boot loader by Xmodem data transfer	*/
/*  HISTORY   : 2006.12.08 Ver 0.01                                    					*/
/*                                                                    	 				*/
/*  Copyright (C) 2006. Renesas Technology Corp.                       					*/
/*  Copyright (C) 2006. Renesas Solutions Corp.                        					*/
/*  All right reserved.                                                					*/
/*                                                                     					*/
/*****************************************************************************************/

#ifndef _COMMAND_H
#define _COMMAND_H

#define	FIRST_USER_FLASH_BLK	4
#define FIRST_USER_FLASH_ADDR	0X2000
//#define LAST_USER_FLASH_ADDR	0xE000
//#define FIRST_USER_FL_ADDR_TXT	"0004000"

#define RESET_VECTOR_OFFSET	0x24


#define INT_RAM_BASE_ADDRESS	0x0800


void InitCommandHandler (void);
void RunCommandHandler (void);
void ShowMenu (void);
void Command_1 (void);
void Command_2 (void);
void Command_3 (void);
void Command_4 (void);
void Command_5 (void);

void reverse(char s[]);
void itoa(long n, char s[], unsigned char base);

typedef unsigned char (*pt2FunctionErase)(unsigned long, unsigned short * );

#define	RAM_BASED_ROM_ADDRESS		0x8000
#define	RAM_BASED_ROM_STOPPER_ADDRESS	0x8400	

#define	SIZE_OF_RAM_BASED_ROM	(RAM_BASED_ROM_STOPPER_ADDRESS-RAM_BASED_ROM_ADDRESS)
#endif

