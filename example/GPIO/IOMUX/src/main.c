/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  * @func  ???��?Y��y?Y��?2����?����????��??oD����a????��?��y??
  * note   ?��??o��?�̨�3???��?��1?��?pin???�䨮??aIO?��������?D����a���騰a��?��?o??��maino����y?a��?��|?����?150MS��?��?��?����?t?������?��
  *        ?a��?����?a ?a����pin???�䨮?3��IO???���??a??��?��?1|?����?��?��??�䨮??aIO??o����???��?1|?����??a?�䨮???o����?IO1|?��?��
  *        ��?1?2??��?��������??��?��pin??��???��?1|?��?��o��?����??????a��?IO1|?����??����?????��?1|?��?1???��DD����?��
  *        ������? NRTS--PA0    SWCLK--IO  SWD--IO  ??��?o??��?��???����????��������??��2��?����?2����������?��2???����?t??o����?
  *        ?��?????������������??��
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "hk32f0301mxxc.h"
#include "hk32f0301mxxc_gpio.h"

static void GpioToggle(void);
static void softWareDelay(void);
static void IOMUXGpioInit(void);
int main(void)
  /* Infinite loop */
{
	softWareDelay();
	IOMUXGpioInit();
  while (1)
  {
		GpioToggle();
		softWareDelay();
  }
}

// TSSOP8 PIN:  2???��y?Y��?2��?DSO8N��?��a���㨤��????D?��a????��?pin
static void IOMUXGpioInit(void)
{
	GPIO_InitTypeDef m_gpio; 
	// ��1?��IOMUX����?��
	RCC_APBPeriph1ClockCmd(RCC_APBPeriph1_IOMUX,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE); 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);

	m_gpio.GPIO_Mode = GPIO_Mode_OUT;
	m_gpio.GPIO_OType = GPIO_OType_PP;
	m_gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	m_gpio.GPIO_Speed = GPIO_Speed_Level_2;
    //	PD5 as GPIO
//	m_gpio.GPIO_Pin = GPIO_Pin_5;
//	GPIO_IOMUX_ChangePin(IOMUX_PIN8,IOMUX_PD5_SEL_PD5);// SOP8  PIN8(PD5) PD5 IO
//	GPIO_Init(GPIOD, &m_gpio);

//	PD5 TO PD1 as GPIO 
	m_gpio.GPIO_Pin = GPIO_Pin_1;  
	GPIO_IOMUX_ChangePin(IOMUX_PIN8,IOMUX_PD5_SEL_PD1);// TSOP8  PIN8(PD5) PD1 IO
	GPIO_Init(GPIOD, &m_gpio);
//	
//	PD5 TO PD3 as GPIO 
//	m_gpio.GPIO_Pin = GPIO_Pin_3;  
//	GPIO_IOMUX_ChangePin(IOMUX_PIN8,IOMUX_PD5_SEL_PD3);// SOP8  PIN8(PD5) PD3 IO
//	GPIO_Init(GPIOD, &m_gpio);
	
//	PD5 TO PC6 as GPIO 
//	m_gpio.GPIO_Pin = GPIO_Pin_6;  
//	GPIO_IOMUX_ChangePin(IOMUX_PIN8,IOMUX_PD5_SEL_PC6);// SOP8 PIN8(PD5) PC6 IO
//	GPIO_Init(GPIOC, &m_gpio);
	
 
	//PD6 TO PA1 as GPIO 
	m_gpio.GPIO_Pin = GPIO_Pin_1; 
	GPIO_IOMUX_ChangePin(IOMUX_PIN1,IOMUX_PD6_SEL_PA1);//SOP8  PIN1(PD6) PA1 IO
	GPIO_Init(GPIOA, &m_gpio);
	
	//	PD6 TO PD4 as GPIO 
//	m_gpio.GPIO_Pin = GPIO_Pin_4; 
//	GPIO_IOMUX_ChangePin(IOMUX_PIN1,IOMUX_PD6_SEL_PD4);// SOP8  PIN1(PD6) PD4 IO
//	GPIO_Init(GPIOD, &m_gpio);
	
	//	PD6 TO PD4 as GPIO 
//	m_gpio.GPIO_Pin = GPIO_Pin_2; 
//	GPIO_IOMUX_ChangePin(IOMUX_PIN1,IOMUX_PD6_SEL_PA2);// SOP8  PIN1(PD6) PA2 IO
//	GPIO_Init(GPIOA, &m_gpio);

 //	Pb5 as GPIO 
//  m_gpio.GPIO_Pin = GPIO_Pin_5; 
//	GPIO_IOMUX_ChangePin(IOMUX_PIN5,IOMUX_PB5_SEL_PB5);//SOP8  PIN5(PB5)PB5 IO
//	GPIO_Init(GPIOB, &m_gpio);
 
  //	Pb5 TO PD2 as GPIO 
//   m_gpio.GPIO_Pin = GPIO_Pin_2; 
//	GPIO_IOMUX_ChangePin(IOMUX_PIN5,IOMUX_PB5_SEL_PD2);//SOP8  PIN5(PB5)PD2 IO
//	GPIO_Init(GPIOD, &m_gpio);
 
  // //	Pb5 TO PA3 as GPIO 
//   m_gpio.GPIO_Pin = GPIO_Pin_3; 
//	GPIO_IOMUX_ChangePin(IOMUX_PIN5,IOMUX_PB5_SEL_PA3);//SOP8 PIN5(PB5)PA3 IO
//	GPIO_Init(GPIOA, &m_gpio);
		 
 //	Pc4 as GPIO 
//  m_gpio.GPIO_Pin = GPIO_Pin_5; 
//	GPIO_IOMUX_ChangePin(IOMUX_PIN6,IOMUX_PC4_SEL_PC5);//SOP8  PIN6(PC4)PC5 IO
//	GPIO_Init(GPIOC, &m_gpio);
 
  //	PC4 TO PC3 as GPIO 
//  m_gpio.GPIO_Pin = GPIO_Pin_3; 
//	GPIO_IOMUX_ChangePin(IOMUX_PIN6,IOMUX_PC4_SEL_PC3);//SOP8  PIN6(PC4)PC3 IO
//	GPIO_Init(GPIOC, &m_gpio);

  // //	PC4 TO PC7 as GPIO 
//   m_gpio.GPIO_Pin = GPIO_Pin_7; 
//	GPIO_IOMUX_ChangePin(IOMUX_PIN6,IOMUX_PC4_SEL_PC7);//SOP8 PIN6(PC4)PC7 IO
//	GPIO_Init(GPIOC, &m_gpio);

	//	nrst -- pa0 
//	m_gpio.GPIO_Pin = GPIO_Pin_0; 
//	GPIO_IOMUX_ChangePin(IOMUX_PIN7,IOMUX_NRST_SEL_PA0);// SOP8 PIN7(NRST)PA0 IO  IOMUX_NRST_SEL_NRST   IOMUX_NRST_SEL_PA0
//	GPIO_Init(GPIOA, &m_gpio);
	
//	m_gpio.GPIO_Pin = GPIO_Pin_4; 
//	GPIO_IOMUX_ChangePin(IOMUX_PIN7,IOMUX_NRST_SEL_PB4);//  IOMUX_NRST_SEL_PB4
//	GPIO_Init(GPIOB, &m_gpio);
}

static void GpioToggle(void)
{
	//GPIOC->ODR ^= GPIO_Pin_4;
	//GPIOC->ODR ^= GPIO_Pin_5;
	//GPIOC->ODR ^= GPIO_Pin_3;
	//GPIOC->ODR ^= GPIO_Pin_6;
	//GPIOC->ODR ^= GPIO_Pin_7;
	GPIOD->ODR ^= GPIO_Pin_1;
	//GPIOD->ODR ^= GPIO_Pin_2;
	//GPIOD->ODR ^= GPIO_Pin_3;
	//GPIOD->ODR ^= GPIO_Pin_4;
	//GPIOD->ODR ^= GPIO_Pin_5;
	//GPIOD->ODR ^= GPIO_Pin_6;	
	//GPIOD->ODR ^= GPIO_Pin_7;
	//GPIOB->ODR ^= GPIO_Pin_5; 
	//GPIOB->ODR ^= GPIO_Pin_4; 
  // GPIOA->ODR ^= GPIO_Pin_0;
	GPIOA->ODR ^= GPIO_Pin_1;
	//GPIOA->ODR ^= GPIO_Pin_2;
	//GPIOA->ODR ^= GPIO_Pin_3; 
}
static void softWareDelay(void)
{
	uint16_t i;
	uint16_t j;
	
	for(i=0;i<500;i++)
	{
		for(j=0;j<700;j++)
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


