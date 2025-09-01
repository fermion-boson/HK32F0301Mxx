/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  * ������ݸù�����Ϊ�ο���д�Ľ���STOPģʽ�ĳ���������ӻ��Ѻ�ε������س����
	* �޷��ٴ�����оƬ,���Գ��԰�ס��λ��(ά��оƬ�ĵ�4������Ϊ��),Ȼ���оƬ�ϵ�,
	* ������Debug������ؿ�ʼ�����оƬID���ɿ���λ�����Ϳ���ʵ��оƬ�ڹ̼��ĸ���
	*
	* �����Ĳ�������: 
	* ������¼�����к�,PC7���������Է�ת,
	* ʵ������Ϊ��WKUP pin Rising ����֮���ٴν���standbyģʽ��Ȼ��WKUP pin Rising �ٴλ���
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "hk32f0301m.h"
#include "hk32f0301m_gpio.h"
#include "usart.h"
#include "string.h" 
#include "mcu.h"
#include "hk32f0301m_exti.h"
#include "hk32f0301m_tim.h"


static void ledGpioInit(void);
static void ledGpioToggle(void);
static void softWareDelay(void); 
 static void ExtiConfigurature(void);
static void PWR_WKUP_CONFIG(void);

uint8_t sysTickIntFlag = 0;
char dataGet;

int main(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	PWR_WKUP_CONFIG();
	ExtiConfigurature();
	ledGpioInit();
	GPIOC->ODR |= GPIO_Pin_7;// disable the led
	while (1)
	{		
		// ÿ�ζ�����ΪWKUP���ѣ���֤ʵ������Ϊ��WKUP pin����֮���ٴν���standbyģʽ��Ȼ���ٴλ���
		PWR_WKUP_CONFIG();
		ExtiConfigurature();
				
		ledGpioToggle();
		softWareDelay();
//		ledGpioToggle();
//		softWareDelay();
//		ledGpioToggle();
//		softWareDelay();  
		PWR_EnterStandbyMode();
    //GPIOC->ODR |= GPIO_Pin_7;// disable the led
		//ledGpioToggle();
		//softWareDelay();
//		ledGpioToggle();
//		softWareDelay();
//		ledGpioToggle();
//		softWareDelay();

	}
}
// PWR �� standbyģʽ weakup pin ���ѵ�����
static void PWR_WKUP_CONFIG(void)
{
	PWR_BackupAccessCmd(ENABLE);  
	// ��������˳���ܽ���
	PWR_WakeUpPinPolarity(RISING);	
	PWR_WakeUpPinCmd(ENABLE);
}

static void ledGpioInit(void)
{
	GPIO_InitTypeDef m_gpio;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	
	m_gpio.GPIO_Mode = GPIO_Mode_OUT;
	m_gpio.GPIO_OType = GPIO_OType_PP;
	m_gpio.GPIO_Pin = GPIO_Pin_7;
	m_gpio.GPIO_PuPd = GPIO_PuPd_UP;
	m_gpio.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOC, &m_gpio);
}

static void ledGpioToggle(void)
{
	GPIOC->ODR ^= GPIO_Pin_7;
}
static void softWareDelay(void)
{
	uint16_t i;
	uint16_t j;
	
	for(i=0;i<100;i++)
	{
		for(j=0;j<500;j++)
		{
			__NOP();
		}
	}
}
 static void ExtiConfigurature(void)
{
	GPIO_InitTypeDef m_gpio;
	EXTI_InitTypeDef m_exti;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	m_gpio.GPIO_Mode = GPIO_Mode_IN;
	m_gpio.GPIO_OType = GPIO_OType_PP;
	m_gpio.GPIO_Pin = GPIO_Pin_4;
	m_gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	m_gpio.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &m_gpio);
	
  m_exti.EXTI_Line = EXTI_Line4;              
	m_exti.EXTI_Mode = EXTI_Mode_Interrupt;      
	m_exti.EXTI_Trigger = EXTI_Trigger_Rising;
	m_exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&m_exti);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource4);
	EXTI_ClearITPendingBit(EXTI_Line4);
	
	NVIC_SetPriority(EXTI4_IRQn,0);
	NVIC_EnableIRQ(EXTI4_IRQn);
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


