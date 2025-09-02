/**************************************************************************************************
* @file    : flash.c
* @brief   : Main program body. Write data to flash with halfword
**************************************************************************************************/

#define __FLASH_C_
/* -----------------------------Includes ----------------------------------------------------------*/

#include "flash.h"
#include "uart.h"

void FlashOptChangeRDP(void)
{
    FLASH_Status eStatus = FLASH_COMPLETE;  
    
   FLASH_Unlock();
    FLASH_OB_Unlock();

    if(FLASH_OB_GetRDP() == SET)
    {
        printf("\r\nFlash RDP change to level0, and system will full chip erase  ");

        FLASH_Unlock();
        FLASH_OB_Unlock();
        eStatus = FLASH_OB_RDPConfig(OB_RDP_Level_0); 
        FLASH_Lock();
        FLASH_OB_Lock();   
         if(eStatus == FLASH_COMPLETE)
        {
            printf("\r\nFlash RDP change to level0 ok.");
            NVIC_SystemReset();

        }
        else
        {
            printf("\r\nFlash RDP change to level0 Failed");
        }
    }
    else
    {
        eStatus = FLASH_OB_RDPConfig(OB_RDP_Level_1);

        FLASH_Lock();
        FLASH_OB_Lock();
        if(eStatus == FLASH_COMPLETE)
        {
            printf("\r\nFlash RDP change to level1 ok. and system will restart ");
       
            NVIC_SystemReset();
        }
        else
        {
            printf("\r\nFlash RDP change to level1  Failed");
        }
            
    }
    
    
}
/* ----------------------------- end of Flash functions -------------------------------------*/
