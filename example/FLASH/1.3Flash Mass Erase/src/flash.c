/**************************************************************************************************
* @file    : flash.c
* @brief   : Main program body. Write data to flash with halfword
**************************************************************************************************/

#define __FLASH_C_
/* -----------------------------Includes ----------------------------------------------------------*/

#include "flash.h"
#include "uart.h"

 /* -----------------------------start of Static Flash functions -------------------------------------*/
static void Flash_Erase(void )
{
    FLASH_Status eStatus = FLASH_COMPLETE;

    FLASH_Unlock();
    eStatus = FLASH_EraseAllPages();
    FLASH_Lock();

    if(eStatus == FLASH_COMPLETE)
    {
        printf("\r\nFlash Erase ok");
    }
    else
    {
        printf("\r\nFlash Erase Failed");
    }
} 


/* ----------------------------- end of Static Flash functions -------------------------------------*/

 /* -----------------------------start of Flash functions -------------------------------------*/

uint16_t Flash_addr = 0;
 
void Flash_PageErase(void)
{ 
    Flash_Erase();
    
}
 

/* ----------------------------- end of Flash functions -------------------------------------*/
