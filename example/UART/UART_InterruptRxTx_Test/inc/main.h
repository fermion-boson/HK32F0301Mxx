/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************

  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

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

#define UART_COM_RX_PIN                 GPIO_Pin_6
#define UART_COM_RX_GPIO_PORT           GPIOD
#define UART_COM_RX_GPIO_CLK            RCC_AHBPeriph_GPIOD
#define UART_COM_RX_SOURCE              GPIO_PinSource6
#define UART_COM_RX_AF                  GPIO_AF_1
   
#define UART_COM_IRQn                   UART1_IRQn







#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

