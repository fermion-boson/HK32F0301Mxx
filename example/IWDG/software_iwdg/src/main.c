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
static __IO uint32_t TimingDelay;

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
    RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_UART1, ENABLE);

    /* Initializes the GPIOx peripheral */
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* selects the pin to used as Alternate function */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);

    /* Initializes the UARTx peripheral */
    UART_InitStructure.UART_BaudRate=115200;
    UART_InitStructure.UART_WordLength=UART_WordLength_8b;
    UART_InitStructure.UART_StopBits=UART_StopBits_1;
    UART_InitStructure.UART_Parity=UART_Parity_No;
    UART_InitStructure.UART_Mode=UART_Mode_Tx;
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
  * @brief  Configure iwdg.
  * @retval None
  */
void IWDG_Config(void)
{
    /* Enable the LSI OSC */ 
    RCC_LSICmd(ENABLE);

    /* Wait till LSI is ready */  
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
    {
    }
  
    /* Enable write access to IWDG_PR and IWDG_RLR��IWDG_WINR registers */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  

    /* IWDG counter clock: LSI/8 */
    IWDG_SetPrescaler(IWDG_Prescaler_8);

	/* Write reload value */
    IWDG_SetReload(0xFFF);

    /* Reload IWDG counter */
    IWDG_ReloadCounter();  

    /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
    IWDG_Enable();
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
	/* Configure uart */
    UART_Config();
	
	/* Configure systick */
	SysTick_init();
	
	/* Configure iwdg */
	IWDG_Config();
	printf("software_IWDG\n");
	
	/* Infinite loop */
    while (1)
    {
        /* LSI=60KHZ,PSC=8,ARR=0xfff, �����ι������ȴ�546ms�ٸ�λ */
        Delay_ms(100);
        /* IWDG feeding dog */
        IWDG_feed();
    }
}

/* �ض���c�⺯��printf�����ڣ��ض�����ʹ��printf���� */
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

