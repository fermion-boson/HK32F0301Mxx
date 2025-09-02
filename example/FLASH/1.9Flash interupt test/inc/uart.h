
#ifndef _UART_h
#define _UART_h

#ifdef __cplusplus
extern "C" {
#endif
	
#include "hk32f0301mxxc.h"
#include "hk32f0301mxxc_gpio.h"
#include "hk32f0301mxxc_uart.h"

#define  UART_IT_EN 0
	
#define  UART1_TX_PORT         GPIOA
#define  UART1_TX_PIN          GPIO_Pin_3
#define  UART1_TX_IO_CLK_EN()  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE)
	
#define  UART1_RX_PORT         GPIOD
#define  UART1_RX_PIN          GPIO_Pin_6
#define  UART1_RX_IO_CLK_EN()  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE)
	
void UART_Configurature(void);
void UART_SendByte(uint8_t ch);
void UART_SendStringLen(uint8_t *p,uint16_t len);
	
      
#if defined __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#elif defined __ICCARM__
  #define PUTCHAR_PROTOTYPE int putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

  
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _UART_h */
