/**************************************************************************************************
* @file    : flash.c
* @brief   : Main program body. Write data to flash with halfword
**************************************************************************************************/

#define __FLASH_C_
/* -----------------------------Includes ----------------------------------------------------------*/

#include "flash.h"
#include "uart.h"


FLASH_Status Flash_Write(uint32_t u32FlashAddress,uint32_t u32pFlashData,uint16_t u16FlashSize)
{
    FLASH_Status eStatus = FLASH_COMPLETE;
    uint8_t i ;

    FLASH_Unlock();
    for ( i = 0; i < u16FlashSize; i +=4)
    {
        eStatus = FLASH_ProgramWord(u32FlashAddress + i,(u32pFlashData)); 
        if(eStatus != FLASH_COMPLETE)
        {
            break;
        }
    } 
    FLASH_Lock();
  return eStatus;

}


//写保护函数
void FlashOptChangeWTP(void)
{
    uint32_t i;
    FLASH_Status eStatus = FLASH_COMPLETE;  
       
    FLASH_Unlock();
    FLASH_OB_Unlock();  

 #if 0
    if((FLASH_OB_GetWRP() & 0xFFFF) == 0xffff)
    {
       printf("\r\nFlash have no write protect");
    }
    else
    { 
        printf("\r\nFlash write protect setting is : 0x%x",FLASH_OB_GetWRP()); 
        eStatus = FLASH_OB_WRPConfig(OB_WRP_None);   
     
        if(eStatus == FLASH_COMPLETE)
        {
            printf("\r\nFlash unwrite protect ok");
            for ( i = 0; i < 0x4000; i+=4)
            {
               eStatus =  Flash_Write(FLASH_OP_ADDR + i, i, 4); 
               if(eStatus != FLASH_COMPLETE)
               {
                   printf("\r\n flash write protect in 0x%08x  \r\n",FLASH_OP_ADDR + i);
                    printf("\r\nFlash write protect Failed");
                   break;
               }
               
            } 
             
        }
        else
        {
             printf("\r\nFlash write protect Failed");
        }        
    } 
     
    printf("\r\nFlash write protect for all pages ");
 
    eStatus = FLASH_OB_WRPConfig(OB_WRP_AllPages); 
    FLASH_Lock();
    FLASH_OB_Lock();   
     if(eStatus == FLASH_COMPLETE)
    { 
        printf("\r\nFlash write protect ok,start to write data ...");
        for ( i = 0; i < 0x4000; i+=4)
        {
            Flash_Write(FLASH_OP_ADDR + i, i, 4);       
            if(eStatus != FLASH_COMPLETE)
               {
                   printf("\r\n flash write protect in 0x%08x  \r\n",FLASH_OP_ADDR + i);
                    printf("\r\nFlash write protect Failed");
                   break;
               }
        } 
    }
    else
    {
        printf("\r\nFlash write protect Failed");
    }
#else
       // 可用单步调试验证 每个函数
    FLASH_OB_Erase();
    FLASH_OB_WRPConfig(OB_WRP_Pages0to1|OB_WRP_Pages14to15|OB_WRP_Pages16to17|OB_WRP_Pages32to33|OB_WRP_Pages46to47|OB_WRP_Pages48to49|OB_WRP_Pages62to63);// 不能分开调用 
 
    FLASH_OB_Erase();
    FLASH_OB_WRPConfig(OB_WRP_AllPages);
    
    FLASH_OB_Erase();
    FLASH_OB_WRPConfig(OB_WRP_None);
    FLASH_Lock();
    FLASH_OB_Lock(); 
    
#endif

    
}
/* ----------------------------- end of Flash functions -------------------------------------*/
