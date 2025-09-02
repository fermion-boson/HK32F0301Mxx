/**************************************************************************************************
* @file    : flash.c
* @brief   : Main program body. Write data to flash with halfword
**************************************************************************************************/

#define __FLASH_C_
/* -----------------------------Includes ----------------------------------------------------------*/

#include "flash.h"
#include "uart.h"

 /* -----------------------------start of Static Flash functions -------------------------------------*/
static void FlashOpt_Erase(void)
{
    FLASH_Status eStatus = FLASH_COMPLETE;

    FLASH_Unlock();
    FLASH_OB_Unlock();
    eStatus = FLASH_OB_Erase(); 
	FLASH_OB_RDPConfig(OB_RDP_Level_0);
    FLASH_Lock();
    FLASH_OB_Lock();
    if(eStatus == FLASH_COMPLETE)
    {
        printf("\r\nFlash option Byte Erase ok");
    }
    else
    {
        printf("\r\nFlash option Byte Erase Failed");
    }
} 


static uint32_t FlashOpt_Read(uint32_t u32FlashAddress)
{
    return *(__IO uint32_t*)(u32FlashAddress);
}

/* ----------------------------- end of Static Flash functions -------------------------------------*/

 /* -----------------------------start of Flash functions -------------------------------------*/

uint16_t Flash_addr = 0;
 
void Flash_PageErase(void)
{
 
    FlashOpt_Erase();
 
}

void Flash_ReadData(void)
{
    for (uint8_t i = 0; i < 7; i++)
    {
        /* code */
        printf("\r\nRead Flash Addr:0x%08x,Data:%d\r\n\r\n", 0x1FFFF000 + i*4, FlashOpt_Read(0x1FFFF000 + i*4)); 
    }


}


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
