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


/* 这里我们使用高级控制定时器TIM1*/

#define            ADVANCE_TIM                   TIM1
#define            ADVANCE_TIM_APBxClock_FUN     RCC_APBPeriph2ClockCmd
#define            ADVANCE_TIM_CLK               RCC_APBPeriph2_TIM1
/* PWM 信号的频率 F = TIM_CLK/{(ARR+1)*(PSC+1)}*/
#define            ADVANCE_TIM_PERIOD            1000
#define            ADVANCE_TIM_PSC               (8-1)//周期1ms
#define            ADVANCE_TIM_PULSE             300

#define            ADVANCE_TIM_IRQ               TIM1_IRQn
#define            ADVANCE_TIM_IRQHandler        TIM1_IRQHandler

/* TIM1 输出比较通道 */
#define            ADVANCE_TIM_CH1_GPIO_CLK      RCC_AHBPeriph_GPIOC
#define            ADVANCE_TIM_CH1_PORT          GPIOC
#define            ADVANCE_TIM_CH1_PIN           GPIO_Pin_6

/*TIM1 输出比较通道的互补通道*/
#define            ADVANCE_TIM_CH1N_GPIO_CLK      RCC_AHBPeriph_GPIOA
#define            ADVANCE_TIM_CH1N_PORT          GPIOA
#define            ADVANCE_TIM_CH1N_PIN           GPIO_Pin_1

/* TIM1 输出比较通道的刹车通道*/
#define            ADVANCE_TIM_BKIN_GPIO_CLK      RCC_AHBPeriph_GPIOB
#define            ADVANCE_TIM_BKIN_PORT          GPIOB
#define            ADVANCE_TIM_BKIN_PIN           GPIO_Pin_5


/* 串口1-USART1*/
#define  DEBUG_UARTx                   UART1
#define  DEBUG_UART_CLK                RCC_APBPeriph2_UART1
#define  DEBUG_UART_APBxClkCmd         RCC_APBPeriph2ClockCmd
#define  DEBUG_UART_BAUDRATE           115200

/* USART GPIO 引脚宏定义*/
#define  DEBUG_UART_GPIO_CLK           (RCC_AHBPeriph_GPIOA)
#define  DEBUG_UART_GPIO_APBxClkCmd    RCC_AHBPeriphClockCmd
    
#define  DEBUG_UART_TX_GPIO_PORT       GPIOA   
#define  DEBUG_UART_TX_GPIO_PIN        GPIO_Pin_3
#define  DEBUG_UART_RX_GPIO_PORT       GPIOB
#define  DEBUG_UART_RX_GPIO_PIN        GPIO_Pin_4

static void ADVANCE_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* 输出比较通道 GPIO 初始化*/
	RCC_AHBPeriphClockCmd(ADVANCE_TIM_CH1_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_CH1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;
  GPIO_Init(ADVANCE_TIM_CH1_PORT, &GPIO_InitStructure);

  /* 输出比较通道互补通道 GPIO 初始化*/
	RCC_AHBPeriphClockCmd(ADVANCE_TIM_CH1N_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_CH1N_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;
  GPIO_Init(ADVANCE_TIM_CH1N_PORT, &GPIO_InitStructure);

  /* 输出比较通道刹车通道 GPIO 初始化*/
	/* BKIN引脚默认先输入低电平*/
	/*因为PB5是SWD引脚，仿真时需要屏蔽此引脚的配置*/
	
	RCC_AHBPeriphClockCmd(ADVANCE_TIM_BKIN_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_BKIN_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(ADVANCE_TIM_BKIN_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_3);
	
	
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_3);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_3);
	
	
}




/* ----------------   PWM信号 周期和占空比的计算--------------- */
/* ARR ：自动重装载寄存器的值
  CLK_cnt：计数器的时钟，等于 Fck_int / (psc+1) = 8M/(psc+1)
  PWM 信号的周期 T = (ARR+1) * (1/CLK_cnt) = (ARR+1)*(PSC+1) / 8M
  占空比P=CCR/(ARR+1)*/

static void ADVANCE_TIM_Mode_Config(void)
{
  /* 开启定时器时钟,即内部时钟CK_INT=8M*/
	ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM_CLK,ENABLE);

/*--------------------时基结构体初始化-------------------------*/
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	/* 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断*/
	TIM_TimeBaseStructure.TIM_Period=ADVANCE_TIM_PERIOD;	
	/* 驱动CNT计数器的时钟 = Fck_int/(psc+1)*/
	TIM_TimeBaseStructure.TIM_Prescaler= ADVANCE_TIM_PSC;	
	/* 时钟分频因子 ，配置死区时间时需要用到*/
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	/* 计数器计数模式，设置为向上计数*/
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	/* 重复计数器的值，没用到不用管*/
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	/* 初始化定时器*/
	TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);

	/*--------------------输出比较结构体初始化-------------------*/		
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	/* 配置为PWM模式1*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	/* 输出使能*/
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	/* 互补输出使能*/
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; 
	/* 设置占空比大小*/
	TIM_OCInitStructure.TIM_Pulse = ADVANCE_TIM_PULSE;
	/* 输出通道电平极性配置*/
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	/* 互补输出通道电平极性配置*/
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	/* 输出通道空闲电平极性配置*/
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	/* 互补输出通道空闲电平极性配置*/
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC1Init(ADVANCE_TIM, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(ADVANCE_TIM, TIM_OCPreload_Enable);

	/*-------------------刹车和死区结构体初始化-------------------*/
	/* 有关刹车和死区结构体的成员具体可参考BDTR寄存器的描述*/
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
  TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
  TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
  TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;
	/* 输出比较信号死区时间配置，具体如何计算可参考 BDTR:UTG[7:0]的描述*/
	/* 这里配置的死区时间为152ns*/
  TIM_BDTRInitStructure.TIM_DeadTime = 11;
  TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
	/* 当BKIN引脚检测到高电平的时候，输出比较信号被禁止，就好像是刹车一样*/
  TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
  TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
  TIM_BDTRConfig(ADVANCE_TIM, &TIM_BDTRInitStructure);
	
	/* 使能计数器*/
	TIM_Cmd(ADVANCE_TIM, ENABLE);	
	/* 主输出使能，当使用的是通用定时器时，这句不需要*/
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

	/* 打开串口GPIO的时钟*/
	DEBUG_UART_GPIO_APBxClkCmd(DEBUG_UART_GPIO_CLK, ENABLE);
	
	/* 打开串口外设的时钟*/
	DEBUG_UART_APBxClkCmd(DEBUG_UART_CLK, ENABLE);

	/* 将USART Tx的GPIO配置为推挽复用模式*/
	GPIO_InitStructure.GPIO_Pin = DEBUG_UART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;
	GPIO_Init(DEBUG_UART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* 将USART Rx的GPIO配置为浮空输入模式*/
	GPIO_InitStructure.GPIO_Pin = DEBUG_UART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(DEBUG_UART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_1);
	
	/* 配置串口的工作参数*/
	/*配置波特率*/
	UART_InitStructure.UART_BaudRate = DEBUG_UART_BAUDRATE;
	/* 配置 针数据字长*/
	UART_InitStructure.UART_WordLength = UART_WordLength_8b;
	/* 配置停止位*/
	UART_InitStructure.UART_StopBits = UART_StopBits_1;
	/* 配置校验位*/
	UART_InitStructure.UART_Parity = UART_Parity_No ;
	
	/* 配置工作模式，收发一起*/
	UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
	/* 完成串口的初始化配置*/
	UART_Init(DEBUG_UARTx, &UART_InitStructure);
	
	/* 使能串口*/
	UART_Cmd(DEBUG_UARTx, ENABLE);	    
}
/*重定向c库函数printf到串口，重定向后可使用printf函数*/
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		UART_SendData(DEBUG_UARTx, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (UART_GetFlagStatus(DEBUG_UARTx, UART_FLAG_TXE) == RESET);		
	
		return (ch);
}

/*为测SYSCFG的lockup*/
uint32_t * p_Hardfault(void)
{

	uint32_t *p;
	return p;
}
 
 
int main(void) 
{
	uint32_t* p1;
	RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_SYSCFG,ENABLE);
	
 /*为测SYSCFG的lockup*/
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


