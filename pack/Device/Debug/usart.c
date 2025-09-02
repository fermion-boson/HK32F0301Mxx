#include "hk32f0301mxxc.h" 
#include "hk32f0301mxxc_uart.h"
#include "hk32f0301mxxc_rcc.h"
#include "systick_delay.h"
#include "usart.h"
#include <stdarg.h>

#define  DEBUG_USART_BAUDRATE     115200

// 此函数仅供参考，示例接收串口数据
void USART1_IRQHandler(void)
{
  uint8_t ucTemp;
  if (UART_GetITStatus(UART1, UART_IT_RXNE) != RESET)
  {
    ucTemp = UART_ReceiveData(UART1);
    UART_SendData(UART1, ucTemp);
    // Usart_printf(UART1, "收到数据rec:%c", ucTemp);
  }
}

void USART_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = UART1_IRQn;
  // value between 0 and 3
  NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief TX:PA3, RX:PB4
 * 
 */
void USART_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	UART_InitTypeDef UART_InitStructure;

	// Enable GPIO and UART clock
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_UART1, ENABLE);
  // AF config
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_1);
	// PA3 As UART TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  // PB4 As UART RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	UART_InitStructure.UART_BaudRate = DEBUG_USART_BAUDRATE;
	// 8-bit
	UART_InitStructure.UART_WordLength = UART_WordLength_8b;
	// 1 stop bit
	UART_InitStructure.UART_StopBits = UART_StopBits_1;
	// No parity
	UART_InitStructure.UART_Parity = UART_Parity_No ;
	// No hardware flow control
	// UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
	// Enable Tx and Rx
	UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
	UART_Init(UART1, &UART_InitStructure);
	
	// UART Interrupt
	USART_NVIC_Configuration();
	UART_ITConfig(UART1, UART_IT_RXNE, ENABLE);	
	// Enable UART1
	UART_Cmd(UART1, ENABLE);	    
}

void Usart_SendByte(UART_TypeDef *pUARTx, uint8_t ch)
{
  /* Send one byte to UART */
  UART_SendData(pUARTx, ch);
  /* Block till tx empty flag is set */
  while (UART_GetFlagStatus(pUARTx, UART_FLAG_TXE) == RESET);
}

void Usart_SendArray( UART_TypeDef * pUARTx, uint8_t *array, uint16_t num)
{
  uint8_t i;
	for(i=0; i<num; i++)
  {
	    Usart_SendByte(pUARTx,array[i]);	
  }
	/* Block till transmission is completed */
	while(UART_GetFlagStatus(pUARTx,UART_FLAG_TC)==RESET);
}

void Usart_SendString( UART_TypeDef * pUARTx, char *str)
{
  while (*str)
  {
    Usart_SendByte( pUARTx, *str++);
  }
  /* Block till transmission is completed */
  while(UART_GetFlagStatus(pUARTx,UART_FLAG_TC)==RESET);
}

void Usart_SendHalfWord( UART_TypeDef * pUARTx, uint16_t ch)
{
  UART_SendData(pUARTx, ch >> 8);
  while (UART_GetFlagStatus(pUARTx, UART_FLAG_TXE) == RESET);
  UART_SendData(pUARTx, ch & 0xFF);
  while (UART_GetFlagStatus(pUARTx, UART_FLAG_TXE) == RESET);
}

void Usart_printf(UART_TypeDef *pUARTx, char *format, ...) {
    va_list args;
    va_start(args, format);

    // 增加缓冲区大小以支持更长的字符串
    char buffer[256]; // 确保缓冲区足够大
    vsnprintf(buffer, sizeof(buffer), format, args); // 格式化字符串

    // 发送格式化后的字符串
    for (uint8_t i = 0; buffer[i] != '\0'; i++) {
        Usart_SendByte(pUARTx, buffer[i]);
    }


    va_end(args);
        // 添加换行符
    Usart_SendByte(pUARTx, '\r');
    Usart_SendByte(pUARTx, '\n');
}