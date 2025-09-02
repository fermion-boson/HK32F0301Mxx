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
#include "stdio.h"
uint16_t TimerPeriod = 0;
uint16_t Channel1Pulse = 0;
extern uint16_t Cap_DutyCycle;

void RCC_Configuration(void);
void GPIO_Configuration(void);
void TIM_Config(void);
void Usart1_Config(uint32_t baud);
void TIM2_config(void);

/****************************printf�����ض���**********************************/
int fputc(int ch,FILE *F) 
{
  UART_SendData(UART1, (uint8_t)ch);
  while (UART_GetFlagStatus(UART1, UART_FLAG_TXE) == RESET)    ;
  return ch;
}

/****************************�����ʱ*****************************************/
void delay(uint16_t time)													
{
	uint16_t  i,j;
	for(i=0;i<time;i++)
          for(j=0;j<200;j++);
}

/***********************Infinite loop *****************************************/
int main(void)
{
	Usart1_Config(115200);
	RCC_Configuration();
	GPIO_Configuration();
	TIM2_config();
	TIM_Config();
  printf("0x55\r\n");
/***********************ѭ����ӡ���벨�ε�ռ�ձ� *****************************/
    while (1)
    {
			
			printf("Capture Wave's DutyCycle is %2d%%\r\n",Cap_DutyCycle);
			delay(10000);
    }
}

/**************************Usart1 ��������************************************/
void Usart1_Config(uint32_t baud)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
	RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_UART1,ENABLE);
/**************************Usart1 TX/RX��������*******************************/
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_1);		
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource4,GPIO_AF_1); 	
/**************************USART1_TX:PA3  USART1_RX:PB4**********************/
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Schmit=GPIO_Schmit_Disable;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_Level_1;
	GPIO_Init(GPIOA,&GPIO_InitStruct);	
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_4;
	GPIO_Init(GPIOB,&GPIO_InitStruct);

	UART_InitTypeDef UART_InitStruct;
	UART_InitStruct.UART_BaudRate=baud;
	UART_InitStruct.UART_Mode=UART_Mode_Rx|UART_Mode_Tx;
	UART_InitStruct.UART_Parity=UART_Parity_No;
	UART_InitStruct.UART_StopBits=UART_StopBits_1;
	UART_InitStruct.UART_WordLength=UART_WordLength_8b;
	UART_Init(UART1,&UART_InitStruct);
	UART_Cmd(UART1,ENABLE);
	
}

/************************����ʱ��**********************************************/
void RCC_Configuration(void)
{
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE );
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOB, ENABLE );
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOC, ENABLE );
	RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_TIM1, ENABLE );
	RCC_APBPeriph1ClockCmd(RCC_APBPeriph1_TIM2, ENABLE );
}
/************************����GPIO**********************************************/
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
/************************����GPIO�ĸ��ù���************************************/
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_3);
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;

		GPIO_Init(GPIOC, &GPIO_InitStructure);
/************************����GPIO�ĸ��ù���************************************/
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource5,GPIO_AF_4);
	}
/*************************����TIMER*********************************************/
void TIM_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef  TIM_ICInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
/*************************Ƶ������Ϊ100KHZ****************************************/
    TimerPeriod = (SystemCoreClock / 100000 ) - 1;

/*************************ʱ������**********************************************/
    TIM_TimeBaseStructure.TIM_Prescaler = 10000;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

			
/*************************����TIM1 Channel1���벶��*****************************/
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; 
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0;
    TIM_ICInit(TIM1, &TIM_ICInitStructure);
		
/*************************����TIM1 Channel1 PWM1���벶��************************/
		TIM_PWMIConfig(TIM1,&TIM_ICInitStructure);
		
		TIM_SelectInputTrigger(TIM1,TIM_TS_TI1FP1);
		TIM_SelectSlaveMode(TIM1,TIM_SlaveMode_Reset);
		
/*************************ʹ��TIM1 CC1�ж�����*********************************/
    TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
/*************************ʹ��TIM1 ������***************************************/
    TIM_Cmd(TIM1, ENABLE);

/*************************����NVIC TIM1ȫ���ж�********************************/
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/************************����TIM2**********************************************/
void TIM2_config(void)
{
	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    
    /* Time Base configuration */
    TIM_TimeBaseStructure.TIM_Prescaler = 100;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 400;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /* Channel 1, 2,3 and 4 Configuration in PWM mode */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 100;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;

    TIM_OC1Init(TIM2, &TIM_OCInitStructure);

    /* TIM1 counter enable */
    TIM_Cmd(TIM2, ENABLE);

    

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


