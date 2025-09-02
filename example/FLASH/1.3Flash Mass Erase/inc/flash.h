#ifndef _FLASH_H
#define _FLASH_H
 
#ifndef __FLASH_C_
#define FLASH_INTERFACE extern
#else
#define FLASH_INTERFACE
#endif/*__FLASH_C_*/
#include "hk32f0301mxxc.h"


#define FLASH_ADDRESS            						0x08001200
#define FLASH_SIZE										0xE00
#define PAGE_SIZE                      					0x00000100
#define FLASH_OP_ADDR									0x08001200 

FLASH_INTERFACE void Flash_PageErase(void);
FLASH_INTERFACE void Flash_ReadData(void);



#endif/*_FLASH_H*/
