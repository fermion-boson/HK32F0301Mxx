/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  * @func  请根据数据手册，来选择自己需要交换的引脚
  * note   在将和系统紧密相关的pin脚复用为IO口时，需要注意，最好在main函数开始处加上100MS以上的软件延时。
  *        这是因为 这类pin脚复用成IO之前，为默认的功能，一旦复用为IO之后，默认功能变为复用之后的IO功能。
  *        如果不加延时，可能pin脚的默认功能会很快被配置为了IO功能，会导致默认功能还没执行完。
  *        比如 NRTS--PA0    SWCLK--IO  SWD--IO  都最好在前面加一段延时，在产品量产时确定不改软件之后，
  *        可将该延时去掉。
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "hk32f0301mxxc.h"
#include "hk32f0301mxxc_gpio.h"

static void GPIO_Config(void);
static void softWareDelay(void);
static void GPIO_Config1(void);
int main(void)
  /* Infinite loop */
{
	softWareDelay();
	GPIO_Config();
  while (1)
  {
		GPIO_Toggle(GPIOD,GPIO_Pin_1);
		softWareDelay(); 
  }
}
static void GPIO_Config(void)
{
	GPIO_InitTypeDef m_gpio;
	

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
	
	m_gpio.GPIO_Mode = GPIO_Mode_OUT;
	m_gpio.GPIO_OType = GPIO_OType_PP;
	m_gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_7;
	m_gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	m_gpio.GPIO_Speed = GPIO_Speed_Level_2;
	GPIO_Init(GPIOA, &m_gpio);
	GPIO_Init(GPIOB, &m_gpio);
	GPIO_Init(GPIOC, &m_gpio);
	GPIO_Init(GPIOD, &m_gpio);
    GPIOA->BRR = GPIO_Pin_All;
    GPIOB->BRR = GPIO_Pin_All;
    GPIOC->BRR = GPIO_Pin_All;
    GPIOD->BRR = GPIO_Pin_All;
	
}
 
static void softWareDelay(void)
{
	uint16_t i;
	uint16_t j;
	
	for(i=0;i<500;i++)
	{
		for(j=0;j<100;j++)
		{
			__NOP();
		}
	}
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


