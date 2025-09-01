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
#include "hk32f0301m.h"
#include "uart.h"


// MCO  gpio  Pc4
void Gpio_MCO_Config(void)
 {
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
  /*---- GPIO */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource4,GPIO_AF_5);

}
//  gpio  Pc7 PC3
void Gpio_Config(void)
 {
  GPIO_InitTypeDef GPIO_InitStructure;
  
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);
  /*---- GPIO PC7*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

   /*---- GPIO PC3 */
  /*
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource3,GPIO_AF_5);
*/
}

int main(void)
{
	uint32_t i , j;
  Uart_Config();
	printf("\r\nMainloop--\r\nMCO Test\r\n");
	Gpio_MCO_Config();
  Gpio_Config();
  RCC_MCOConfig(RCC_MCOSource_SYSCLK,RCC_MCOPrescaler_1);

  while (1)
  {
      /* Infinite loop */
		for(i = 0; i < 10; i++)
    	for(j = 0; j < 0x0fff; j++)
        { }
    GPIO_Toggle(GPIOC,GPIO_Pin_7);
  }
}



// printf redefine
int fputc(int ch,FILE *f)
{

	USART_SendData(USART1, (uint8_t) ch);
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
  {}		
  return ch;
}
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
#endif /* USE_FULL_ASSERT */


