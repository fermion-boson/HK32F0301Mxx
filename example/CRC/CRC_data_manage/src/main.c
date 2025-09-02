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


uint32_t CRC_value32=0;
uint32_t CRC_value16=0;
uint32_t  CRC_value8=0;

#define DataWith32 32
#define DataWith16 16
#define DataWith8   8

#define DataWith   DataWith8

void RCC_Configuration(void);
void GPIO_Configuration(void);
void UART_Configuration(void);


void delay(uint32_t i)
{
	while(i--);
}


 
int main(void)
  /* Infinite loop */
{
	RCC_Configuration();
	GPIO_Configuration();
	UART_Configuration();

	
	CRC_DeInit();
	
	printf("开始CRC校验\n"); 
	//RCC_MCOConfig(RCC_MCOSource_SYSCLK,RCC_MCOPrescaler_1);

	/*处理32位数据*/
	if(DataWith==DataWith32)
	{
		CRC_CalcCRC(0x12345678);
		CRC_value32=CRC_GetCRC();
		printf("32-bit CRC===  0X%X\r\n",CRC_value32);
	}
	/*处理16位数据*/
	else if(DataWith==DataWith16)
	{
		/*输入CRC计算工具的值是1234，而不是 00001234。多项式依旧是32位04C11DB7*/
		CRC_CalcCRC16bits(0x1234);
		CRC_value16=CRC_GetCRC();
		printf("16-bit CRC==== 0X%X\r\n",CRC_value16);
	}
	/*处理8位数据*/
	else if(DataWith==DataWith8)
	{
		/*输入CRC计算工具的值是12，而不是 00000012。多项式依旧是32位04C11DB7*/
		CRC_CalcCRC8bits(0x12);
		CRC_value8=CRC_GetCRC();
		printf("8-bit CRC===== 0X%X\r\n",CRC_value8);
	}

  while (1)
  {

  }
	
}


void RCC_Configuration(void)
{
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_CRC, ENABLE );
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE );
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOB, ENABLE );
  RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_UART1,ENABLE);

}

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*PA3，TX*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_1);	
	/*PB4，RX*/
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_1);

}


void UART_Configuration(void)
{
	UART_InitTypeDef UART_InitStructure;
	
	UART_InitStructure.UART_BaudRate = 115200;
  UART_InitStructure.UART_WordLength = UART_WordLength_8b;
  UART_InitStructure.UART_StopBits = UART_StopBits_1;
  UART_InitStructure.UART_Parity = UART_Parity_No;
  UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
	UART_Init(UART1, &UART_InitStructure);
  UART_Cmd(UART1, ENABLE);

}

int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		UART_SendData(UART1, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (UART_GetFlagStatus(UART1, UART_FLAG_TXE) == RESET);		
	
		return (ch);
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


