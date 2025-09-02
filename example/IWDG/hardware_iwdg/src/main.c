/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "hk32f0301mxxc.h"

/* Delay count */
static  __IO uint32_t TimingDelay;
OB_TypeDef OB_Types;

/**
  * @brief  Configure uart.
  * @retval None
  */
void UART_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;

	/*  Enables clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA , ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB , ENABLE);
    RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_UART1, ENABLE);

	/* Initializes the GPIOx peripheral */
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Initializes the GPIOx peripheral */
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init( GPIOD, &GPIO_InitStructure);

    /* selects the pin to used as Alternate function */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_1);

    /* Initializes the UARTx peripheral */
    UART_InitStructure.UART_BaudRate=115200;
    UART_InitStructure.UART_WordLength=UART_WordLength_8b;
    UART_InitStructure.UART_StopBits=UART_StopBits_1;
    UART_InitStructure.UART_Parity=UART_Parity_No;
    UART_InitStructure.UART_Mode=UART_Mode_Rx|UART_Mode_Tx;
    UART_Init(UART1, &UART_InitStructure);

    /* Enables UARTx peripheral */
    UART_Cmd(UART1,  ENABLE);
}

/**
  * @brief  Configure systick period to 1ms.
  * @retval None
  */
void SysTick_init(void)
{
	if(SysTick_Config(SystemCoreClock/1000))
	{
		while(1);
	}	
}

/**
  * @brief  Configure hardware IWDG.
  * @retval None
  */
void Hardware_iwdg(void)
{
	/* Unlocks the FLASH control register and program memory access */
    FLASH_Unlock();
	/* Unlocks the option bytes block access */
    FLASH_OB_Unlock();
    
    /* 擦除 选项字之前先读出保存*/
    OB_Types.USER_RDP = OB->USER_RDP;
    OB_Types.USER_RDP &= 0xFFF0FFFF;
    OB_Types.USER_RDP |= (uint32_t)((uint16_t)(OB_IWDG_HW | OB_STOP_RST)<<16);
    
    //OB_Types.IWDG_CFGR  = (uint32_t)((0x5B1E<<16)|0x0FFF);
    OB_Types.IWDG_CFGR  = OB->IWDG_CFGR;
    OB_Types.DATA = OB->DATA;
    OB_Types.WRP01 = OB->WRP01;
    OB_Types.WRP23 = OB->WRP23; 
    OB_Types.DBG_CLK_LSI_LP_CTL = OB->DBG_CLK_LSI_LP_CTL;
    OB_Types.NRST_SEL_KEY_BOR_LVL = OB->NRST_SEL_KEY_BOR_LVL;
    
	/* Erases the FLASH option bytes */
	FLASH_OB_Erase();
    
    /* 修改硬件看门狗后重新加载所有选项字 */
    FLASH_OB_ProgramData(0X1FFFF000,OB_Types.USER_RDP);
    FLASH_OB_ProgramData(0X1FFFF004,OB_Types.DATA);
    FLASH_OB_ProgramData(0X1FFFF008,OB_Types.WRP01);
    FLASH_OB_ProgramData(0X1FFFF00c,OB_Types.WRP23);
    FLASH_OB_ProgramData(0X1FFFF010,OB_Types.IWDG_CFGR);
    FLASH_OB_ProgramData(0X1FFFF014,OB_Types.DBG_CLK_LSI_LP_CTL);
    FLASH_OB_ProgramData(0X1FFFF018,OB_Types.NRST_SEL_KEY_BOR_LVL);
    
	/* Locks the option bytes block access */
    FLASH_OB_Lock();
	/* Locks the Program memory access */
    FLASH_Lock();
}

/**
  * @brief  IWDG feeding dog.
  * @retval None
  */
void IWDG_feed(void)
{
	IWDG_ReloadCounter();
	printf("feed dog\n");
}

/**
  * @brief  Millisecond delay function.
  * @param  nTime: delay time(ms).
  * @retval None
  */
void Delay_ms(__IO uint32_t nTime)
{
	TimingDelay=nTime;
	while(TimingDelay !=0);
}

/**
  * @brief  Delay count.
  * @retval None
  */
void TimingDelay_Decrement(void)
{
    if(TimingDelay !=0x00)
    {
        TimingDelay--;
    }
}

/**
  * @brief  Main program
  * @retval None
  */
int main(void)
{
	/* Configure systick */
	SysTick_init();
	
	/* Configure hardware IWDG */
	Hardware_iwdg();
    
    /* Configure uart */
    UART_Config();
	
	printf("IWDG硬件看门狗\n");
	
	/* Infinite loop */
    while (1)
    {	  
        Delay_ms(50);
		/* IWDG feeding dog */
        IWDG_feed();
    }
}

/* 重定向c库函数printf到串口，重定向后可使用printf函数 */
PUTCHAR_PROTOTYPE
{
    /* Transmits single data through the UARTx peripheral */
    UART_SendData(UART1, (uint8_t) ch);
	
    /* wait for sending */
    while(UART_GetFlagStatus(UART1, UART_FLAG_TXE)==RESET);

    return(ch);
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char* file , uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */	
    /* Infinite loop */
	while (1)
    {		
    }
}
#endif /* USE_FULL_ASSERT */


