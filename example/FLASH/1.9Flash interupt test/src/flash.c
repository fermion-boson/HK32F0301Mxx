/**************************************************************************************************
* @file    : flash.c
* @brief   : Main program body. Write data to flash with halfword
**************************************************************************************************/

#define __FLASH_C_
/* -----------------------------Includes ----------------------------------------------------------*/

#include "flash.h"
#include "uart.h"

 /* -----------------------------start of Static Flash functions -------------------------------------*/
static void Flash_Erase(uint32_t u32FlashAddress)
{
    FLASH_Status eStatus = FLASH_COMPLETE;

    FLASH_Unlock();
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

static void Flash_Write(uint32_t u32FlashAddress,uint8_t *u8pFlashData,uint16_t u16FlashSize)
{
    FLASH_Status eStatus = FLASH_COMPLETE;
    uint8_t i ;

    FLASH_Unlock();
    for ( i = 0; i < u16FlashSize; i +=4)
    {
        eStatus = FLASH_ProgramWord(u32FlashAddress + i,u8pFlashData[i]); 
        if(eStatus != FLASH_COMPLETE)
        {
            break;
        }
    } 
    FLASH_Lock();

    if(eStatus != FLASH_COMPLETE)
    {
        printf("\r\nFlash Program Failed at:0x%08x",u32FlashAddress + i);
    }
    else
    {
        printf("\r\nFlash Program ok");
    }   

}

static uint32_t Flash_Read(uint32_t u32FlashAddress)
{
    return *(__IO uint32_t*)(u32FlashAddress);
}

/* ----------------------------- end of Static Flash functions -------------------------------------*/

 /* -----------------------------start of Flash functions -------------------------------------*/
DATA_BUFF DataBuf;
uint16_t Flash_addr = 0;
 
void Flash_WriteData(void)
{
    if((Flash_addr % PAGE_SIZE) == 0x00)
    {
        printf("Flash_offset:%03x ",Flash_addr);
		printf(",Flash_Erase:0x%08x, ",FLASH_OP_ADDR + Flash_addr);
		Flash_Erase(FLASH_OP_ADDR + Flash_addr);
    }
    printf(",Write Data:%d ",DataBuf.u32Data);
	Flash_Write(FLASH_OP_ADDR + Flash_addr, &DataBuf.u8Data[0],4);	
    // pgerr测试
   // printf("\r\nFlash Program error test");
    //Flash_Write(FLASH_OP_ADDR + Flash_addr, &DataBuf.u8Data[0],4);	
}

void Flash_ReadData(void)
{
    printf(",Read Flash Addr:0x%08x,Data:%d\r\n\r\n",FLASH_OP_ADDR + Flash_addr, Flash_Read(FLASH_OP_ADDR + Flash_addr)); 
	Flash_addr += 0x04;
	
	if(Flash_addr >= FLASH_SIZE)
	{
		Flash_addr = 0; 
	}
}

/* ----------------------------- end of Flash functions -------------------------------------*/
