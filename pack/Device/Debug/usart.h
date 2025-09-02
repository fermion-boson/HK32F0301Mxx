#ifndef USART_H
#define USART_H

#include "hk32f0301mxxc.h"
#include "hk32f0301mxxc_uart.h"


// 函数声明
void USART_Configuration(void);
// 发送函数声明
void Usart_SendByte(UART_TypeDef *pUARTx, uint8_t ch);
void Usart_SendArray(UART_TypeDef *pUARTx, uint8_t *array, uint16_t num);
void Usart_SendString(UART_TypeDef *pUARTx, char *str);
void Usart_SendHalfWord(UART_TypeDef *pUARTx, uint16_t ch);
void Usart_printf(UART_TypeDef * pUARTx,char *str,...);

// 定义 DEBUG_PRINT
#ifdef DEBUG_PRINTF_ENABLE
#define DEBUG_PRINTF(fmt,...) Usart_printf(UART1,fmt,##__VA_ARGS__);
#else
#define DEBUG_PRINTF(fmt,...)
#endif
#endif // USART_H