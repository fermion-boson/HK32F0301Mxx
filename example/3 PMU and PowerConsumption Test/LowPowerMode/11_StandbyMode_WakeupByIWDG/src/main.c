/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  * 如果根据该工程作为参考改写的进入Standby模式的程序忘记添加唤醒后段导致下载程序后
	* 无法再次连接芯片,或以尝试按住复位键(维持芯片的第4个引脚为低),然后给芯片上电,
	* 再连接Debug点击下载开始后读到芯片ID后松开复位按键就可以实现芯片内固件的更新
	*
	* 正常的测试现象: 
	* 由IWDG复位信号唤醒
	* 程序烧录并运行后,PC7引脚周期性翻转,且高电平时间远小于低电平维持时间
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
static void IWDG_Config(uint8_t prv ,uint16_t rlv);

uint8_t sysTickIntFlag = 0;
char dataGet;

int main(void)
{ 
	USART_Configurature();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	// IWDG 1s 左右的超时溢出，
  IWDG_Config(IWDG_Prescaler_64 ,625);
	ledGpioInit();
	printf("Main loop!\r\n");
	while (1)
	{		
		PWR_EnterStandbyMode();
		ledGpioToggle();
		softWareDelay();
	}
}

// 溢出时间 Tout = prv/40 * rlv (s)
// 计数器时钟频率(40K主频) CK_CNT= 40/ 4*2^PRV，一个计数器时钟周期 计数器就减一
static void IWDG_Config(uint8_t prv ,uint16_t rlv)
{
  // 使能 预分频寄存器 PR 和重装载寄存器 RLR 可写
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  // 设置预分频器值
  IWDG_SetPrescaler( prv );
  // 设置重装载寄存器值
  IWDG_SetReload( rlv );
  // 把重装载寄存器的值放到计数器中
  IWDG_ReloadCounter();  
  // 使能 IWDG
  IWDG_Enable();
}

static void ledGpioInit(void)
{
	GPIO_InitTypeDef m_gpio;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	
	m_gpio.GPIO_Mode = GPIO_Mode_OUT;
	m_gpio.GPIO_OType = GPIO_OType_PP;
	m_gpio.GPIO_Pin = GPIO_Pin_7;
	m_gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
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


