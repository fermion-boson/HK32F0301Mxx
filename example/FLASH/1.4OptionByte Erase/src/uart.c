
/**************************************************************************************************
* @file      UART.c
**************************************************************************************************/

#include "uart.h"
#include "stdio.h"
#include "stdarg.h"

static void UART_GPIO_Configurature(void);
#if (UART_IT_EN == 1)
static void UART_NVIC_Configurature(void);
#endif

void UART_Configurature(void)
{
	UART_InitTypeDef m_UART;
	
	UART_GPIO_Configurature();
	
	RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_UART1, ENABLE);
	m_UART.UART_BaudRate = 115200; 
	m_UART.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
	m_UART.UART_Parity = UART_Parity_No;
	m_UART.UART_StopBits = UART_StopBits_1;
	m_UART.UART_WordLength = UART_WordLength_8b;
	UART_Init(UART1, &m_UART); 
	UART_Cmd(UART1, ENABLE);

#if (UART_IT_EN == 1)	
	UART_ITConfig(UART1, UART_IT_RXNE, ENABLE);
	
	UART_NVIC_Configurature();
#endif
}

void UART_SendByte(uint8_t ch)
{
	while((UART1->ISR & UART_ISR_TXE) == 0);
	UART1->TDR = ch;
}

void UART_SendStringLen(uint8_t *p,uint16_t len)
{
	while(len--)
	{
		UART_SendByte(*(p++));
	}
}

static void UART_GPIO_Configurature(void)
{
	GPIO_InitTypeDef m_gpio;
	
	UART1_TX_IO_CLK_EN();
	UART1_RX_IO_CLK_EN();
	
	m_gpio.GPIO_Mode = GPIO_Mode_AF;
	m_gpio.GPIO_OType = GPIO_OType_PP;
	m_gpio.GPIO_Pin = UART1_TX_PIN;
	m_gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	m_gpio.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(UART1_TX_PORT, &m_gpio);
	GPIO_PinAFConfig(UART1_TX_PORT,GPIO_PinSource3,GPIO_AF_1);
	
	m_gpio.GPIO_Pin = UART1_RX_PIN;
	GPIO_Init(UART1_RX_PORT, &m_gpio);
	GPIO_PinAFConfig(UART1_RX_PORT,GPIO_PinSource6,GPIO_AF_1);
}

#if (UART_IT_EN == 1)
static void UART_NVIC_Configurature(void)
{
	NVIC_SetPriority(UART1_IRQn,0);
	NVIC_EnableIRQ(UART1_IRQn);
}

void UART1_IRQHandler(void)
{
	uint8_t ch_rx_test;
	if((UART1->ISR & UART_ISR_RXNE) != 0)
	{
		ch_rx_test = UART1->RDR;
		UART_SendByte(ch_rx_test);
	}
}
#endif

/*******************************************************************************************************
* @function: fputc
* @brief:    over-write fputc function.
*******************************************************************************************************/
 
PUTCHAR_PROTOTYPE
{
    /*  Printf   */
    UART_SendData(UART1, (unsigned char) ch);
    while(UART_GetFlagStatus(UART1, UART_FLAG_TC) != SET);
    return (ch);
}
/*******************************************************************************************************
* @function: fgetc
* @brief:    over-write fgetc function.
*******************************************************************************************************/
int fgetc(FILE *f)
{
	while((UART1->ISR & UART_ISR_RXNE) == 0); 
	return (int)((uint8_t)(UART1->RDR));
}
