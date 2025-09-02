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

#define ADC1_DR_Address    ((uint32_t)0x40012440)
uint16_t ADCBuffer[3] = {0};
uint32_t ADCcapture;
uint8_t sample_flag=0;

void RCC_Configuration(void);
void GPIO_Configuration(void);

void USART_Configuration(void);
void ADC_Configuration(void);
void TIM_Config(void);
void GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
static void softWareDelay(void);
void softWareDelay1(void);


int main(void)
  /* Infinite loop */
{
	RCC_Configuration();
	GPIO_Configuration();
	USART_Configuration();
	TIM_Config();
	ADC_Configuration();
	

  printf("开始ADC实验\n");
	softWareDelay();	

  while (1)
  {
		  softWareDelay();	
			if(sample_flag==1)
			{
				printf("ADC=====%d\n",ADC->DR);
				sample_flag=0;
			}

			
  }
}

void softWareDelay(void)
{
	uint16_t i;
	uint16_t j;
	
	for(i=0;i<100;i++)
	{
		for(j=0;j<80;j++)
		{
			
		}
	}
}

void RCC_Configuration(void)
{
	

	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE );
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOB, ENABLE );
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOC, ENABLE );
  RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_UART1,ENABLE);

	RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_ADC ,ENABLE);
	RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_TIM1,ENABLE);
	RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_SYSCFG,ENABLE);
 
}

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*配置模拟IO*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	

	/*PA3，TX*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_1);	
	/*PB4，RX*/
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_1);
		
}

		
/*翻转IO*/
void GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  /* Check the parameters */
  assert_param(IS_GPIO_PIN(GPIO_Pin));

  GPIOx->ODR ^= GPIO_Pin;
}

/*配置TIMER*/
void TIM_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef        TIM_OCInitStructure;
  /* TIM1 DeInit */
  TIM_DeInit(TIM1);
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_OCStructInit(&TIM_OCInitStructure); 
 
	/* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 1000;
  TIM_TimeBaseStructure.TIM_Prescaler = 8-1;  
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
  
	
  /* Output Compare PWM Mode configuration */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;           
  TIM_OCInitStructure.TIM_Pulse = 500;
  TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	
	TIM_SelectOutputTrigger(TIM1 ,TIM_TRGOSource_Update);
	
  /* TIM1 enable counter */
  TIM_Cmd(TIM1, ENABLE);
  
  /* Main Output Enable */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
	
	
	NVIC_InitTypeDef NVIC_InitStructure; 
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TRG_COM_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);

}
void ADC_Configuration(void)
{
  ADC_InitTypeDef ADC_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	ADC_DeInit(ADC);
	ADC_StructInit(&ADC_InitStructure);
	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Falling;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC4;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;

	ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
  ADC_Init(ADC,&ADC_InitStructure);
	
  /* ADC1 regular channels configuration */ 
	ADC_ChannelConfig(ADC, ADC_Channel_2 , ADC_SampleTime_1_5Cycles);

	NVIC_InitStructure.NVIC_IRQChannel = ADC1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
  ADC_ITConfig(ADC ,ADC_IT_EOC,ENABLE);
	
	
  ADC_GetCalibrationFactor(ADC);
	ADC_ClearFlag(ADC, ADC_FLAG_ADRDY);

	
	/* Enable ADC1 */
  ADC_Cmd(ADC, ENABLE);
	while(!ADC_GetFlagStatus(ADC, ADC_FLAG_ADRDY)); 	
	
	ADC_StartOfConversion(ADC);
}

void USART_Configuration(void)
{
	UART_InitTypeDef UART_InitStructure;
	
	UART_InitStructure.UART_BaudRate = 115200;
  UART_InitStructure.UART_WordLength = UART_WordLength_8b;
  UART_InitStructure.UART_StopBits = UART_StopBits_1;
  UART_InitStructure.UART_Parity = UART_Parity_No;
  UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
	UART_Init(UART1, &UART_InitStructure);
  UART_Cmd(UART1, ENABLE);

}

int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		UART_SendData(UART1, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (UART_GetFlagStatus(UART1, UART_FLAG_TXE) == RESET);		
	
		return (ch);
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

