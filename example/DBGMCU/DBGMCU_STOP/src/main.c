/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "hk32f0301mxxc.h"

#define FALSE	0
#define TRUE	1
	
void Exti_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /*GPIO 时钟使能*/
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    //  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    /*---- GPIO PA3*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;   //GPIO_Pin_3

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* 配置EXTI */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Line = EXTI_Line3;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource3);
    EXTI_ClearITPendingBit(EXTI_Line3);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}
 
int main(void)
{
    uint32_t cnt = 0;

    RCC_APBPeriph1ClockCmd(RCC_APBPeriph1_PWR,ENABLE);   // Enable PWR clock 

    Exti_Config();

    for (cnt = 0; cnt < 500000; cnt++){__NOP();__NOP();}
    //增加dbgmcu_stop模式
    RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_DBGMCU,ENABLE);
    DBGMCU_Config(DBGMCU_STOP, ENABLE);

    PWR_EnterStopMode(PWR_Regulator_LowPower, PWR_Entry_WFI);

    while (1)
    {
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


