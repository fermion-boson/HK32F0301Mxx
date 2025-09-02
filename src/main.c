#define DEBUG_PRINTF_ENABLE // Enable debug print
#include "main.h"
#include "hk32f0301mxxc.h"
#include "hk32f0301mxxc_gpio.h"
#include "systick_delay.h"
#include "uart.h"

static void ledGpioInit(void);
static void ledGpioToggle(void);
int main(void)
/* Infinite loop */
{
    SysTick_Init();
    UARTx_Init();
    ledGpioInit();
    while (1) {
        ledGpioToggle();
        SysTick_DelayMs(1000);
        DEBUG_PRINTF("Hello, HK32F0301MXX");
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
    m_gpio.GPIO_Speed = GPIO_Speed_Level_3;
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
void assert_failed(char *file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: DEBUG_PRINTF("Wrong parameters value: file %s on line %d\r\n", file, line) */
    // DEBUG_PRINTF("Wrong parameters value: file %s on line %d\r\n", file, line)
    /* Infinite loop */
    while (1) {
    }
}
#endif