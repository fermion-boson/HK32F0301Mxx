/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 */

/* Includes ------------------------------------------------------------------*/
#define DEBUG_PRINTF_ENABLE  // Enable debug print
#include "main.h"
#include "hk32f0301m.h"
#include "hk32f0301m_gpio.h"
#include "systick_delay.h"
#include "usart.h"


static void ledGpioInit(void);
static void ledGpioToggle(void);
int main(void)
/* Infinite loop */
{
	SysTick_Init();
	USART_Configuration();
	DEBUG_PRINTF("Debug USART Printf Example");
    ledGpioInit();
    while (1) {
        ledGpioToggle();
        SysTick_DelayMs(1000);
		DEBUG_PRINTF("LED TOGGLE");
    }
}
static void ledGpioInit(void)
{
    GPIO_InitTypeDef m_gpio;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

    m_gpio.GPIO_Mode  = GPIO_Mode_OUT;
    m_gpio.GPIO_OType = GPIO_OType_PP;
    m_gpio.GPIO_Pin   = GPIO_Pin_7;
    m_gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    m_gpio.GPIO_Speed = GPIO_Speed_Level_2;
    GPIO_Init(GPIOC, &m_gpio);
}

static void ledGpioToggle(void)
{
    GPIOC->ODR ^= GPIO_Pin_7;
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
#endif /* USE_FULL_ASSERT */
