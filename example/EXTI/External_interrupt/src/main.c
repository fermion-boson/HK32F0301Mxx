/********************************************************************
* @Copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd
* @Filename:  main.c
* @brief  :   Main program body
* @Author :   AE Team
* @Version:   V1.0.0/2022-10-31
* 1.初始版本
 ********************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "define.h"
#include "main.h"
#include "hk32f0301mxxc.h"

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
#define FALSE	0
#define TRUE	1

void Exti_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /*GPIO 时钟使能*/
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_SYSCFG, ENABLE);

    /*---- GPIO PA3*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;   //GPIO_Pin_3

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource3);
    EXTI_ClearITPendingBit(EXTI_Line3);

    /* 配置EXTI */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Line = EXTI_Line3;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

void Led_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

int main(void)
{
    Exti_Config();
    Led_Init();

    while (1)
    {
    }
}



/**
  * @}
  */

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
#endif
/**
  * @}
  */



