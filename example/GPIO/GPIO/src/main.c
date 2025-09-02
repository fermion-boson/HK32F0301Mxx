/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  * @func  ����������ֲᣬ��ѡ���Լ���Ҫ����������
  * note   �ڽ���ϵͳ������ص�pin�Ÿ���ΪIO��ʱ����Ҫע�⣬�����main������ʼ������100MS���ϵ������ʱ��
  *        ������Ϊ ����pin�Ÿ��ó�IO֮ǰ��ΪĬ�ϵĹ��ܣ�һ������ΪIO֮��Ĭ�Ϲ��ܱ�Ϊ����֮���IO���ܡ�
  *        ���������ʱ������pin�ŵ�Ĭ�Ϲ��ܻ�ܿ챻����Ϊ��IO���ܣ��ᵼ��Ĭ�Ϲ��ܻ�ûִ���ꡣ
  *        ���� NRTS--PA0    SWCLK--IO  SWD--IO  �������ǰ���һ����ʱ���ڲ�Ʒ����ʱȷ���������֮��
  *        �ɽ�����ʱȥ����
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


