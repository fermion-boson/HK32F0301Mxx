#ifndef _FLASH_H
#define _FLASH_H
 
#ifndef __FLASH_C_
#define FLASH_INTERFACE extern
#else
#define FLASH_INTERFACE
#endif/*__FLASH_C_*/
#include "hk32f0301mxxc.h"

 
 
#define FLASH_OPT_ADDR									0x1FFFF800

FLASH_INTERFACE void Flash_PageErase(void);
FLASH_INTERFACE void Flash_ReadData(void);
FLASH_INTERFACE void FlashOptChangeRDP(void);
 

#endif/*_FLASH_H*/
