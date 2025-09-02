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


/* ��������ʹ�ø߼����ƶ�ʱ��TIM1*/

#define            ADVANCE_TIM                   TIM1
#define            ADVANCE_TIM_APBxClock_FUN     RCC_APBPeriph2ClockCmd
#define            ADVANCE_TIM_CLK               RCC_APBPeriph2_TIM1
/* PWM �źŵ�Ƶ�� F = TIM_CLK/{(ARR+1)*(PSC+1)}*/
#define            ADVANCE_TIM_PERIOD            1000
#define            ADVANCE_TIM_PSC               (8-1)//����1ms
#define            ADVANCE_TIM_PULSE             300

#define            ADVANCE_TIM_IRQ               TIM1_IRQn
#define            ADVANCE_TIM_IRQHandler        TIM1_IRQHandler

/* TIM1 ����Ƚ�ͨ�� */
#define            ADVANCE_TIM_CH1_GPIO_CLK      RCC_AHBPeriph_GPIOC
#define            ADVANCE_TIM_CH1_PORT          GPIOC
#define            ADVANCE_TIM_CH1_PIN           GPIO_Pin_6

/*TIM1 ����Ƚ�ͨ���Ļ���ͨ��*/
#define            ADVANCE_TIM_CH1N_GPIO_CLK      RCC_AHBPeriph_GPIOA
#define            ADVANCE_TIM_CH1N_PORT          GPIOA
#define            ADVANCE_TIM_CH1N_PIN           GPIO_Pin_1

/* TIM1 ����Ƚ�ͨ����ɲ��ͨ��*/
#define            ADVANCE_TIM_BKIN_GPIO_CLK      RCC_AHBPeriph_GPIOB
#define            ADVANCE_TIM_BKIN_PORT          GPIOB
#define            ADVANCE_TIM_BKIN_PIN           GPIO_Pin_5


/* ����1-USART1*/
#define  DEBUG_UARTx                   UART1
#define  DEBUG_UART_CLK                RCC_APBPeriph2_UART1
#define  DEBUG_UART_APBxClkCmd         RCC_APBPeriph2ClockCmd
#define  DEBUG_UART_BAUDRATE           115200

/* USART GPIO ���ź궨��*/
#define  DEBUG_UART_GPIO_CLK           (RCC_AHBPeriph_GPIOA)
#define  DEBUG_UART_GPIO_APBxClkCmd    RCC_AHBPeriphClockCmd
    
#define  DEBUG_UART_TX_GPIO_PORT       GPIOA   
#define  DEBUG_UART_TX_GPIO_PIN        GPIO_Pin_3
#define  DEBUG_UART_RX_GPIO_PORT       GPIOB
#define  DEBUG_UART_RX_GPIO_PIN        GPIO_Pin_4

static void ADVANCE_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* ����Ƚ�ͨ�� GPIO ��ʼ��*/
	RCC_AHBPeriphClockCmd(ADVANCE_TIM_CH1_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_CH1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;
  GPIO_Init(ADVANCE_TIM_CH1_PORT, &GPIO_InitStructure);

  /* ����Ƚ�ͨ������ͨ�� GPIO ��ʼ��*/
	RCC_AHBPeriphClockCmd(ADVANCE_TIM_CH1N_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_CH1N_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;
  GPIO_Init(ADVANCE_TIM_CH1N_PORT, &GPIO_InitStructure);

  /* ����Ƚ�ͨ��ɲ��ͨ�� GPIO ��ʼ��*/
	/* BKIN����Ĭ��������͵�ƽ*/
	/*��ΪPB5��SWD���ţ�����ʱ��Ҫ���δ����ŵ�����*/
	
	RCC_AHBPeriphClockCmd(ADVANCE_TIM_BKIN_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_BKIN_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(ADVANCE_TIM_BKIN_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_3);
	
	
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_3);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_3);
	
	
}




/* ----------------   PWM�ź� ���ں�ռ�ձȵļ���--------------- */
/* ARR ���Զ���װ�ؼĴ�����ֵ
  CLK_cnt����������ʱ�ӣ����� Fck_int / (psc+1) = 8M/(psc+1)
  PWM �źŵ����� T = (ARR+1) * (1/CLK_cnt) = (ARR+1)*(PSC+1) / 8M
  ռ�ձ�P=CCR/(ARR+1)*/

static void ADVANCE_TIM_Mode_Config(void)
{
  /* ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=8M*/
	ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM_CLK,ENABLE);

/*--------------------ʱ���ṹ���ʼ��-------------------------*/
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	/* �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�*/
	TIM_TimeBaseStructure.TIM_Period=ADVANCE_TIM_PERIOD;	
	/* ����CNT��������ʱ�� = Fck_int/(psc+1)*/
	TIM_TimeBaseStructure.TIM_Prescaler= ADVANCE_TIM_PSC;	
	/* ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�*/
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	/* ����������ģʽ������Ϊ���ϼ���*/
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	/* �ظ���������ֵ��û�õ����ù�*/
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	/* ��ʼ����ʱ��*/
	TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);

	/*--------------------����ȽϽṹ���ʼ��-------------------*/		
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	/* ����ΪPWMģʽ1*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	/* ���ʹ��*/
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	/* �������ʹ��*/
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; 
	/* ����ռ�ձȴ�С*/
	TIM_OCInitStructure.TIM_Pulse = ADVANCE_TIM_PULSE;
	/* ���ͨ����ƽ��������*/
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	/* �������ͨ����ƽ��������*/
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	/* ���ͨ�����е�ƽ��������*/
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	/* �������ͨ�����е�ƽ��������*/
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC1Init(ADVANCE_TIM, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(ADVANCE_TIM, TIM_OCPreload_Enable);

	/*-------------------ɲ���������ṹ���ʼ��-------------------*/
	/* �й�ɲ���������ṹ��ĳ�Ա����ɲο�BDTR�Ĵ���������*/
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
  TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
  TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
  TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;
	/* ����Ƚ��ź�����ʱ�����ã�������μ���ɲο� BDTR:UTG[7:0]������*/
	/* �������õ�����ʱ��Ϊ152ns*/
  TIM_BDTRInitStructure.TIM_DeadTime = 11;
  TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
	/* ��BKIN���ż�⵽�ߵ�ƽ��ʱ������Ƚ��źű���ֹ���ͺ�����ɲ��һ��*/
  TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
  TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
  TIM_BDTRConfig(ADVANCE_TIM, &TIM_BDTRInitStructure);
	
	/* ʹ�ܼ�����*/
	TIM_Cmd(ADVANCE_TIM, ENABLE);	
	/* �����ʹ�ܣ���ʹ�õ���ͨ�ö�ʱ��ʱ����䲻��Ҫ*/
	TIM_CtrlPWMOutputs(ADVANCE_TIM, ENABLE);
}

void ADVANCE_TIM_Init(void)
{
	ADVANCE_TIM_GPIO_Config();
	ADVANCE_TIM_Mode_Config();		
}


void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	UART_InitTypeDef UART_InitStructure;

	/* �򿪴���GPIO��ʱ��*/
	DEBUG_UART_GPIO_APBxClkCmd(DEBUG_UART_GPIO_CLK, ENABLE);
	
	/* �򿪴��������ʱ��*/
	DEBUG_UART_APBxClkCmd(DEBUG_UART_CLK, ENABLE);

	/* ��USART Tx��GPIO����Ϊ���츴��ģʽ*/
	GPIO_InitStructure.GPIO_Pin = DEBUG_UART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;
	GPIO_Init(DEBUG_UART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* ��USART Rx��GPIO����Ϊ��������ģʽ*/
	GPIO_InitStructure.GPIO_Pin = DEBUG_UART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(DEBUG_UART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_1);
	
	/* ���ô��ڵĹ�������*/
	/*���ò�����*/
	UART_InitStructure.UART_BaudRate = DEBUG_UART_BAUDRATE;
	/* ���� �������ֳ�*/
	UART_InitStructure.UART_WordLength = UART_WordLength_8b;
	/* ����ֹͣλ*/
	UART_InitStructure.UART_StopBits = UART_StopBits_1;
	/* ����У��λ*/
	UART_InitStructure.UART_Parity = UART_Parity_No ;
	
	/* ���ù���ģʽ���շ�һ��*/
	UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
	/* ��ɴ��ڵĳ�ʼ������*/
	UART_Init(DEBUG_UARTx, &UART_InitStructure);
	
	/* ʹ�ܴ���*/
	UART_Cmd(DEBUG_UARTx, ENABLE);	    
}
/*�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����*/
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ����� */
		UART_SendData(DEBUG_UARTx, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (UART_GetFlagStatus(DEBUG_UARTx, UART_FLAG_TXE) == RESET);		
	
		return (ch);
}

/*Ϊ��SYSCFG��lockup*/
uint32_t * p_Hardfault(void)
{

	uint32_t *p;
	return p;
}
 
 
int main(void) 
{
	uint32_t* p1;
	RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_SYSCFG,ENABLE);
	
 /*Ϊ��SYSCFG��lockup*/
	SYSCFG->CFGR1=1<<31;
	
	
  ADVANCE_TIM_Init();
	
	USART_Config();
	printf("123\n");
	
	p1 = p_Hardfault();
	*p1 = 30;
	
	
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


