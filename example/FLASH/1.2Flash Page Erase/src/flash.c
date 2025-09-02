/**************************************************************************************************
* @file    : flash.c
* @brief   : Main program body. Write data to flash with halfword
**************************************************************************************************/

#define __FLASH_C_
/* -----------------------------Includes ----------------------------------------------------------*/

#include "flash.h"
#include "UART.h"

 /* -----------------------------start of Static Flash functions -------------------------------------*/
static void Flash_Erase(uint32_t u32FlashAddress)
{
    FLASH_Status eStatus = FLASH_COMPLETE;

    FLASH_Unlock();
    FLASH_OB_Unlock();
    eStatus = FLASH_ErasePage(u32FlashAddress);
    FLASH_Lock();

    if(eStatus == FLASH_COMPLETE)
    {
        printf("\r\nFlash Erase ok at:0x%08x",u32FlashAddress);
    }
    else
    {
        printf("\r\nFlash Erase Failed at:0x%08x",u32FlashAddress);
    }
} 

static void Flash_Write(uint32_t u32FlashAddress,uint32_t u32pFlashData)
{
    FLASH_Status eStatus = FLASH_COMPLETE;

    FLASH_Unlock();

    eStatus = FLASH_ProgramWord(u32FlashAddress,u32pFlashData); 
    if(eStatus != FLASH_COMPLETE)
    {
        return ;
    }
  
    FLASH_Lock(); 

}
static uint32_t Flash_Read(uint32_t u32FlashAddress)
{
    return *(__IO uint32_t*)(u32FlashAddress);
}

/* ----------------------------- end of Static Flash functions -------------------------------------*/

 /* -----------------------------start of Flash functions -------------------------------------*/

uint16_t Flash_addr = 0;
 
void Flash_PageErase(void)
{
    uint16_t index ; 
    for(index = 0 ; index < 0x300; )
    {
       if((Flash_addr % PAGE_SIZE) == 0x00)
        {
            printf("\r\n Flash_offset:0x%08x ",Flash_addr);
            printf(",Flash_Erase:0x%08x, ",FLASH_OP_ADDR + Flash_addr);
            Flash_Erase(FLASH_OP_ADDR + Flash_addr);
            index += 0x100;
            Flash_addr += 0x100;
        }
    }

}

void Flash_ReadData(void)
{   
    uint32_t index ; 
 
	for(index = 0 ; index < 0x300; index++)
    {
        Flash_Write(FLASH_OP_ADDR+ index*4,index);  
    }
    for(index = 0 ; index < 0x300; )
    {
        printf("\r\nRead Flash Addr:0x%08x,Data:0x%08x\r\n\r\n",FLASH_OP_ADDR + index, Flash_Read(FLASH_OP_ADDR + index)); 
        index += 4;
    }
    // test for flash erase adress at 256 offset
      printf("\r\n test for flash erase adress at %d offset",0x1FF);
      printf(",Flash_Erase:0x%08x, ",FLASH_OP_ADDR + 0x1FF);
      Flash_Erase(FLASH_OP_ADDR + 0x1FF);
      for(index = 0 ; index < 0x300; )
        {
            printf("\r\nRead Flash Addr:0x%08x,Data:0x%08x\r\n\r\n",FLASH_OP_ADDR + index, Flash_Read(FLASH_OP_ADDR + index)); 
            index += 4;
        }
    
    while(1)
    {}
}

/* ----------------------------- end of Flash functions -------------------------------------*/
