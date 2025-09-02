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


uint32_t DeviceUID[3] = {0};

void RCC_Configuration(void);
void GPIO_Configuration(void);
void UART_Configuration(void);
void Get96BIT_UID(void)
{ 
    Sys_GetDevice96BitUID(&DeviceUID[0]);
    printf("DeviceUID UID[0]:0x%08x\r\n",DeviceUID[0]);
    printf("DeviceUID UID[1]:0x%08x\r\n",DeviceUID[1]);
	  printf("DeviceUID UID[2]:0x%08x\r\n",DeviceUID[2]);
	
}

 
int main(void)
  /* Infinite loop */
{
	uint32_t i , j;
	RCC_Configuration();
	GPIO_Configuration();
	UART_Configuration();
	
	printf("\r\nMainloop--\r\n96BIT_UID Test\r\n");

	//mcro_config();//--------------

  while (1)
  {
      /* Infinite loop */
		for(i = 0; i < 10; i++)
    	for(j = 0; j < 0x0fff; j++)
      
        { }
    Get96BIT_UID();
  }
	
	
}

void RCC_Configuration(void)
{
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


