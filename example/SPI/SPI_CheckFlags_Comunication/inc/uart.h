#ifndef _UART_H_
#define _UART_H_

#include "hk32f0301mxxc.h"



/** 
  * @{
  */

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

/**
  * @}
  */
  
#if defined __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#elif defined __ICCARM__
  #define PUTCHAR_PROTOTYPE int putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


void UART_COMInit(UART_InitTypeDef* UART_InitStruct);
void UART_printf(UART_TypeDef* UARTx, const char *Data,...);
uint8_t UART_Scanf(uint32_t value, uint32_t count);

#endif

