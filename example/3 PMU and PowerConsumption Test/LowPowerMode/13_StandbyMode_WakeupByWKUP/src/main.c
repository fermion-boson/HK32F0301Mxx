/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  * 如果根据该工程作为参考改写的进入STOP模式的程序忘记添加唤醒后段导致下载程序后
	* 无法再次连接芯片,或以尝试按住复位键(维持芯片的第4个引脚为低),然后给芯片上电,
	* 再连接Debug点击下载开始后读到芯片ID后松开复位按键就可以实现芯片内固件的更新
	*
	* 正常的测试现象: 
	* 程序烧录并运行后,PC7引脚周期性翻转,
	* 实验现象为，WKUP pin Rising 唤醒之后，再次进入standby模式，然后WKUP pin Rising 再次唤醒
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
		// 每次都配置为WKUP唤醒，保证实验现象为，WKUP pin唤醒之后，再次进入standby模式，然后再次唤醒
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
// PWR 的 standby模式 weakup pin 唤醒的配置
static void PWR_WKUP_CONFIG(void)
{
	PWR_BackupAccessCmd(ENABLE);  
	// 下面两个顺序不能交换
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


