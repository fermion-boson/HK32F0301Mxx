/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : uart.h
  * @brief          : Header for uart.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************

  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_H
#define __UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hk32f0301mxxc.h"

/**
 * @brief Definition for COM port1, connected to UART1
 */ 
#define UART_COM                        UART1
#define UART_COM_CLK                    RCC_APBPeriph2_UART1

#define UART_COM_TX_PIN                 GPIO_Pin_3
#define UART_COM_TX_GPIO_PORT           GPIOA
#define UART_COM_TX_GPIO_CLK            RCC_AHBPeriph_GPIOA
#define UART_COM_TX_SOURCE              GPIO_PinSource3
#define UART_COM_TX_AF                  GPIO_AF_1

#define UART_COM_RX_PIN                 GPIO_Pin_4
#define UART_COM_RX_GPIO_PORT           GPIOB
#define UART_COM_RX_GPIO_CLK            RCC_AHBPeriph_GPIOB
#define UART_COM_RX_SOURCE              GPIO_PinSource4
#define UART_COM_RX_AF                  GPIO_AF_1
   
#define UART_COM_IRQn                   UART1_IRQn

void UARTx_Init(void);
void UART_SendByte(uint8_t data);
void UART_SendString(char* str);
void UART_SendHalfWord(uint16_t ch);
void UART_SendArray(uint8_t* array, uint16_t num);
void UART_Printf(char* format, ...);

// 定义 DEBUG_PRINT
// #ifdef DEBUG_PRINTF_ENABLE
#define DEBUG_PRINTF(fmt,...) UART_Printf(fmt,##__VA_ARGS__);
// #else
// #define DEBUG_PRINTF(fmt,...)
// #endif

#ifdef __cplusplus
}
#endif

#endif /* __UART_H */