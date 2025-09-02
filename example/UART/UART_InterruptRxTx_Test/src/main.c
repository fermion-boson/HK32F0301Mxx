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

static void UART_Config(void);
static void NVIC_Config(void);


int main(void)
{
    UART_Config();
    NVIC_Config();
  
    UART_SendData(UART1, 0xAA);
    while(UART_GetFlagStatus(UART_COM, UART_FLAG_TC) != SET);
    UART_SendData(UART1, 0xBB);
    while(UART_GetFlagStatus(UART_COM, UART_FLAG_TC) != SET);

    while (1)
    {

    }
}


static void UART_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  UART_InitTypeDef UART_InitStructure;

  /* Enable GPIO clock */
  RCC_AHBPeriphClockCmd(UART_COM_TX_GPIO_CLK | UART_COM_RX_GPIO_CLK, ENABLE);

  /* Enable UART clock */
  RCC_APBPeriph2ClockCmd(UART_COM_CLK, ENABLE); 

  /* Connect PXx to UARTx_Tx */
  GPIO_PinAFConfig(UART_COM_TX_GPIO_PORT, UART_COM_TX_SOURCE, UART_COM_TX_AF);

  /* Connect PXx to UARTx_Rx */
  GPIO_PinAFConfig(UART_COM_RX_GPIO_PORT, UART_COM_RX_SOURCE, UART_COM_RX_AF);
  
  /* Configure UART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = UART_COM_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(UART_COM_TX_GPIO_PORT, &GPIO_InitStructure);
    
  /* Configure UART Rx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = UART_COM_RX_PIN;
  GPIO_Init(UART_COM_RX_GPIO_PORT, &GPIO_InitStructure);

  /* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  UART_InitStructure.UART_BaudRate = 115200;
  UART_InitStructure.UART_WordLength = UART_WordLength_8b;
  UART_InitStructure.UART_StopBits = UART_StopBits_1;
  UART_InitStructure.UART_Parity = UART_Parity_No;
  UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;

  /* UART configuration */
  UART_Init(UART_COM, &UART_InitStructure);
  
  UART_ITConfig(UART_COM, UART_IT_RXNE, ENABLE);
    
  /* Enable UART */
  UART_Cmd(UART_COM, ENABLE);
}

static void NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_InitStructure.NVIC_IRQChannel = UART_COM_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
  NVIC_Init(&NVIC_InitStructure);
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


