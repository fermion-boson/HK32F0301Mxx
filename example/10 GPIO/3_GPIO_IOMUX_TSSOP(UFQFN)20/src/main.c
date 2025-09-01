/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  * @func  ����������ֲᣬ��ѡ���Լ���Ҫ����������
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "hk32f0301m.h"
#include "hk32f0301m_gpio.h"

static void GpioInit(void);
static void GpioToggle(void);
static void softWareDelay(void);
static void IOMUXGpioInit(void);
int main(void)
  /* Infinite loop */
{
softWareDelay();
	GpioInit();
	IOMUXGpioInit();
  while (1)
  {
		GpioToggle();
		softWareDelay();
  }
}
static void GpioInit(void)
{
	GPIO_InitTypeDef m_gpio;
	

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	
	m_gpio.GPIO_Mode = GPIO_Mode_OUT;
	m_gpio.GPIO_OType = GPIO_OType_PP;
	m_gpio.GPIO_Pin = GPIO_Pin_7;
	m_gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	m_gpio.GPIO_Speed = GPIO_Speed_Level_2;
	GPIO_Init(GPIOC, &m_gpio);
	
}
// TSSOP20 PIN
static void IOMUXGpioInit(void)
{
	GPIO_InitTypeDef m_gpio; 
	// ʹ��IOMUXʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_IOMUX,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE); 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
//	PD5 as GPIO
	m_gpio.GPIO_Mode = GPIO_Mode_OUT;
	m_gpio.GPIO_OType = GPIO_OType_PP;
	m_gpio.GPIO_Pin = GPIO_Pin_5;
	m_gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	m_gpio.GPIO_Speed = GPIO_Speed_Level_2;
	GPIO_IOMUX_ChangePin(IOMUX_PIN2,IOMUX_PD5_SEL_PD5);// TSSOP20 PIN2(PD5) PD5 IO
	GPIO_Init(GPIOD, &m_gpio);
//	
// //	Pb5 as GPIO 
  m_gpio.GPIO_Pin = GPIO_Pin_5; 
	GPIO_IOMUX_ChangePin(IOMUX_PIN11,IOMUX_PB5_SEL_PB5);//TSSOP20PIN11(PD5)PB5 IO
	GPIO_Init(GPIOB, &m_gpio);
 
	//	nrst -- pa0 
	//m_gpio.GPIO_Pin = GPIO_Pin_0; 
	//GPIO_IOMUX_ChangePin(IOMUX_PIN4,IOMUX_NRST_SEL_NRST);// TSSOP20PIN4(NRST)PA0 IO  IOMUX_NRST_SEL_NRST   IOMUX_NRST_SEL_PA0
	//GPIO_Init(GPIOA, &m_gpio);
}

static void GpioToggle(void)
{
	GPIOC->ODR ^= GPIO_Pin_4;
	GPIOC->ODR ^= GPIO_Pin_5;
	GPIOC->ODR ^= GPIO_Pin_3;
	GPIOC->ODR ^= GPIO_Pin_6;
	GPIOC->ODR ^= GPIO_Pin_7;
	GPIOD->ODR ^= GPIO_Pin_1;
	GPIOD->ODR ^= GPIO_Pin_2;
	GPIOD->ODR ^= GPIO_Pin_3;
	GPIOD->ODR ^= GPIO_Pin_4;
	GPIOD->ODR ^= GPIO_Pin_5;
	GPIOD->ODR ^= GPIO_Pin_6;	
	GPIOD->ODR ^= GPIO_Pin_7;
	GPIOB->ODR ^= GPIO_Pin_5; 
	GPIOB->ODR ^= GPIO_Pin_4; 
	GPIOA->ODR ^= GPIO_Pin_1;
	GPIOA->ODR ^= GPIO_Pin_2;
	GPIOA->ODR ^= GPIO_Pin_3; 
}
static void softWareDelay(void)
{
	uint16_t i;
	uint16_t j;
	
	for(i=0;i<500;i++)
	{
		for(j=0;j<1000;j++)
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
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
#endif /* USE_FULL_ASSERT */


