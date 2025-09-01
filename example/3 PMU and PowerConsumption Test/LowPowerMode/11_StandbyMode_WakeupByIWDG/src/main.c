/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  * ������ݸù�����Ϊ�ο���д�Ľ���Standbyģʽ�ĳ���������ӻ��Ѻ�ε������س����
	* �޷��ٴ�����оƬ,���Գ��԰�ס��λ��(ά��оƬ�ĵ�4������Ϊ��),Ȼ���оƬ�ϵ�,
	* ������Debug������ؿ�ʼ�����оƬID���ɿ���λ�����Ϳ���ʵ��оƬ�ڹ̼��ĸ���
	*
	* �����Ĳ�������: 
	* ��IWDG��λ�źŻ���
	* ������¼�����к�,PC7���������Է�ת,�Ҹߵ�ƽʱ��ԶС�ڵ͵�ƽά��ʱ��
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
	// IWDG 1s ���ҵĳ�ʱ�����
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

// ���ʱ�� Tout = prv/40 * rlv (s)
// ������ʱ��Ƶ��(40K��Ƶ) CK_CNT= 40/ 4*2^PRV��һ��������ʱ������ �������ͼ�һ
static void IWDG_Config(uint8_t prv ,uint16_t rlv)
{
  // ʹ�� Ԥ��Ƶ�Ĵ��� PR ����װ�ؼĴ��� RLR ��д
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  // ����Ԥ��Ƶ��ֵ
  IWDG_SetPrescaler( prv );
  // ������װ�ؼĴ���ֵ
  IWDG_SetReload( rlv );
  // ����װ�ؼĴ�����ֵ�ŵ���������
  IWDG_ReloadCounter();  
  // ʹ�� IWDG
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


