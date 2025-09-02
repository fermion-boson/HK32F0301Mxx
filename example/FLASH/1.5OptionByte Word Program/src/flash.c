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

static void FlashOpt_Write(uint32_t u32FlashAddress,uint32_t u32pFlashData)
{
    FLASH_Status eStatus = FLASH_COMPLETE;
    FLASH_Unlock();
    FLASH_OB_Unlock();
    eStatus = FLASH_OB_ProgramData(u32FlashAddress,u32pFlashData); 
    if(eStatus != FLASH_COMPLETE)
    {
        return;
    }
  
    FLASH_OB_Lock();
    FLASH_Lock();
    if(eStatus != FLASH_COMPLETE)
    {
        printf("\r\nFlash Program Failed at:0x%08x",u32FlashAddress);
    }
    else
    {
        printf("\r\nFlash Program ok");
    }   

}

static uint32_t FlashOpt_Read(uint32_t u32FlashAddress)
{
    return *(__IO uint32_t*)(u32FlashAddress);
}

/* ----------------------------- end of Static Flash functions -------------------------------------*/

 /* -----------------------------start of Flash functions -------------------------------------*/


 
void Flash_PageErase(void)
{ 
    uint8_t u8EraseCnt ;
    // 然后再全部擦除，之后再写入
    FlashOpt_Erase(); 
    
    for(u8EraseCnt = 4; u8EraseCnt < PAGE_SIZE;)    
   {
       FlashOpt_Write(FLASH_OPT_ADDR + u8EraseCnt,0x00ff00ff);
        u8EraseCnt+=4;
   }
 
}

void Flash_ReadData(void)
{
    for (uint8_t i = 0; i < 0x1C; )
    {
        /* code */
        printf(",Read Flash Addr:0x%08x,Data:0x%08x\r\n\r\n",FLASH_OPT_ADDR + i, FlashOpt_Read(FLASH_OPT_ADDR + i));  
		i += 4;
    }
  
}


/* ----------------------------- end of Flash functions -------------------------------------*/
