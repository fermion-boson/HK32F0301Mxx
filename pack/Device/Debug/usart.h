#ifndef USART_H
#define USART_H

#include "hk32f0301m.h"




// 函数声明
void USART_Configuration(void);
// 发送函数声明
void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch);
void Usart_SendArray(USART_TypeDef *pUSARTx, uint8_t *array, uint16_t num);
void Usart_SendString(USART_TypeDef *pUSARTx, char *str);
void Usart_SendHalfWord(USART_TypeDef *pUSARTx, uint16_t ch);
void Usart_printf(USART_TypeDef * pUSARTx,char *str,...);

// 定义 DEBUG_PRINT
#ifdef DEBUG_PRINTF_ENABLE
#define DEBUG_PRINTF(fmt,...) Usart_printf(USART1,fmt,##__VA_ARGS__);
#else
#define DEBUG_PRINTF(fmt,...)
#endif
#endif // USART_H