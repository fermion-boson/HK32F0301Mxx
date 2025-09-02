#include "hk32f0301m.h" 
#include "systick_delay.h"
#include "usart.h"
#include <stdarg.h>

#define  DEBUG_USART_BAUDRATE     115200

// 此函数仅供参考，示例接收串口数据
void USART1_IRQHandler(void)
{
  uint8_t ucTemp;
  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
    ucTemp = USART_ReceiveData(USART1);
    USART_SendData(USART1, ucTemp);
    // Usart_printf(USART1, "收到数据rec:%c", ucTemp);
  }
}

void USART_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
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
	USART_InitTypeDef USART_InitStructure;

	// Enable GPIO and UART clock
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  // AF config
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_1);
	// PA3 As USART TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  // PB4 As USART RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	// 8-bit
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 1 stop bit
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// No parity
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// No hardware flow control
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// Enable Tx and Rx
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	// UART Interrupt
	USART_NVIC_Configuration();
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	
	// Enable UART1
	USART_Cmd(USART1, ENABLE);	    
}

void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch)
{
  /* Send one byte to USART */
  USART_SendData(pUSARTx, ch);
  /* Block till tx empty flag is set */
  while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
  uint8_t i;
	for(i=0; i<num; i++)
  {
	    Usart_SendByte(pUSARTx,array[i]);	
  }
	/* Block till transmission is completed */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
  while (*str)
  {
    Usart_SendByte( pUSARTx, *str++);
  }
  /* Block till transmission is completed */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
  USART_SendData(pUSARTx, ch >> 8);
  while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
  USART_SendData(pUSARTx, ch & 0xFF);
  while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

void Usart_printf(USART_TypeDef *pUSARTx, char *format, ...) {
    va_list args;
    va_start(args, format);

    // 增加缓冲区大小以支持更长的字符串
    char buffer[256]; // 确保缓冲区足够大
    vsnprintf(buffer, sizeof(buffer), format, args); // 格式化字符串

    // 发送格式化后的字符串
    for (uint8_t i = 0; buffer[i] != '\0'; i++) {
        Usart_SendByte(pUSARTx, buffer[i]);
    }


    va_end(args);
        // 添加换行符
    Usart_SendByte(pUSARTx, '\r');
    Usart_SendByte(pUSARTx, '\n');
}
