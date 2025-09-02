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
#include "hk32f0301mxxc_wwdg.h"

uint8_t WWDG_Tr,WWDG_Wr;

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
    UART_Cmd(UART1, ENABLE);
}

/**
  * @brief  Configure WWDG NVIC.
  * @retval None
  */
void WWDG_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel=WWDG_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority=0;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Configure WWDG.
  * @retval None
  */
void WWDG_Config(void)
{
	/* Enables clock */
    RCC_APBPeriph1ClockCmd(RCC_APBPeriph1_WWDG,ENABLE);
	
	/* Sets the WWDG Prescaler */
    WWDG_SetPrescaler(WWDG_Prescaler_8);
	
	/* Sets the WWDG window value */
    WWDG_SetWindowValue(WWDG_WINDOW_VALUE);
	
	/* Enables WWDG and load the counter value */
    WWDG_Enable(WWDG_COUNT_VALUE);

	/* Clears Early Wakeup interrupt flag */
    WWDG_ClearFlag();

	/* Configure WWDG NVIC */
    WWDG_NVIC_Config();

	/* Enables the WWDG Early Wakeup interrupt */
    WWDG_EnableIT();	
}

/**
  * @brief  WWDG feeding dog.
  * @retval None
  */
void WWDG_feed(void)
{
	WWDG_SetCounter(WWDG_COUNT_VALUE);
    printf("feed dog\n");
}

/**
  * @brief  Main program
  * @retval None
  */
int main(void) 
{
	/* Configure uart */
    UART_Config();

	/* Configure WWDG */
	WWDG_Config();
	
    printf("WWDG \n");
	
    /* window value */
	WWDG_Wr = (WWDG->CFR&0X7F);
	
	/* Infinite loop */
    while (1)
    {
        /* counter value */
        WWDG_Tr = (WWDG->CR&0X7F);
      
        /* Feed the dog at counter value smaller than the window value */
		if(WWDG_Tr < WWDG_Wr)
        {
            WWDG_feed();
        }
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


