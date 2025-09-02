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
#include "hk32f0301mxxc_gpio.h"
#include <stdio.h>
#include "stdarg.h"

ADC_InitTypeDef ADC_InitStructure;
UART_InitTypeDef UART_InitStructure;
NVIC_InitTypeDef    NVIC_InitStructure;

uint32_t  ADCConvertedValue = 0;
static void softWareDelay(void);
float ADCConvertedVoltage = 0;
void RCC_Configuration(void);
void GPIO_Configuration(void);
void ADC_Configuration(void);
void UART_Configuration(void);
int fputc(int ch, FILE *f);
void UART1_printf(UART_TypeDef* UARTx, uint8_t *Data,...);

int main(void)
  /* Infinite loop */
{
	RCC_Configuration();
	GPIO_Configuration();
	ADC_Configuration();
	UART_Configuration();
	softWareDelay();
  while (1)
  {
		softWareDelay();
		while(ADC_GetFlagStatus(ADC, ADC_FLAG_EOC) == RESET);
		ADCConvertedValue =ADC_GetConversionValue(ADC);
		ADCConvertedVoltage = (ADCConvertedValue *3.3)/4095;//计算ADC转换结果（电压值）
		printf("ADC_DR register of CH2:0x%04X \n",ADCConvertedValue);//打印寄存器值
		printf("ADC_V of CH2:%f V\n",ADCConvertedVoltage);//打印电压
  }
}
/*软件延时*/
static void softWareDelay(void)
{
	uint16_t i;
	uint16_t j;
	
	for(i=0;i<2000;i++)
	{
		for(j=0;j<1000;j++)
		{
			__NOP();
		}
	}
}
/*时钟初始化*/
void RCC_Configuration(void)
{
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOD, ENABLE );
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE );
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOC, ENABLE );
	RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_UART1,ENABLE);
	RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_ADC ,ENABLE);
}
/*IO初始化*/
void GPIO_Configuration(void)
{
	//初始化模拟IO PC4
	GPIO_InitTypeDef GPIO_InitStructure;
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource4,GPIO_AF_7);
	//PD3用于触发
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource3,GPIO_AF_7);	
	
	//初始化串口IO
	//PD6，RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_1);	
	//PA3，TX
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_1);

}

/*ADC配置*/
void ADC_Configuration(void)
{

	ADC_DeInit(ADC);
	ADC_StructInit(&ADC_InitStructure);
	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising; 
  ADC_InitStructure.ADC_ExternalTrigConv =  ADC_ExternalTrigConv_IO_TRGO; //ADC外部IO触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
  ADC_Init(ADC,&ADC_InitStructure);
  /* ADC1 regular channels configuration */ 
  ADC_ChannelConfig(ADC, ADC_Channel_2 , ADC_SampleTime_239_5Cycles);
//  ADC_GetCalibrationFactor(ADC1);

  /* Enable ADC1 */
  ADC_Cmd(ADC, ENABLE);

	while(!ADC_GetFlagStatus(ADC, ADC_FLAG_ADRDY)); 
	ADC_StartOfConversion(ADC);
}
/*串口配置*/
void UART_Configuration(void)
{
	UART_InitStructure.UART_BaudRate = 115200;
  UART_InitStructure.UART_WordLength = UART_WordLength_8b;
  UART_InitStructure.UART_StopBits = UART_StopBits_1;
  UART_InitStructure.UART_Parity = UART_Parity_No;
//  UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
  UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
	UART_Init(UART1, &UART_InitStructure);

	UART_ITConfig(UART1,UART_IT_RXNE,ENABLE);//串口发送完成中断
  UART_Cmd(UART1, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = UART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	UART_ITConfig(UART1, UART_IT_RXNE, ENABLE);
	UART_ITConfig(UART1, UART_IT_IDLE, ENABLE);
	NVIC_Init(&NVIC_InitStructure);
	
}
/*printf重定义*/
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		UART_SendData(UART1, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (UART_GetFlagStatus(UART1, UART_FLAG_TXE) == RESET);		
	
		return (ch);
}
/*
 * 函数名：itoa
 * 描述  ：将整型数据转换成字符串
 * 输入  ：-radix=10 表示十进制，其他结果为0
           -value 要转换的整型数
 *         -string 转换后的字符串
 * 输出  ：无
 * 调用  ：由UART1_printf()调用
 */
static char *itoa(int value, char *string, int radix)
{
    int  i,d;
    int  flag = 0;
    char *ptr = string;
    
    /* 此处仅支持十进制转换 */
    if(radix != 10)
    {
        *ptr = 0;
        return string;
    }
    
    if(!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }
    
    /* 如果是一个负号，就加上“-”符号 */
    if(value < 0)
    {
        *ptr++ = '-';
        value *= -1;
    }
    for(i=10000; i>0; i/=10)
    {
        d = value / i;
        if(d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }
    
    /* 空终止字符. */
    *ptr = 0;
    return string;
}

/*
 * 函数名：UART1_printf
 * 描述  ：格式化输出，类似于C 库中的printf，但这里没有用到C库
 * 输入  ：-UARTx 串口通道，这里只用到了串口1，即UART1
 *         -Data 要发送到串口的内容的指针
 * 输出  ：无
 * 调用  ：外部调用
 */
void UART1_printf(UART_TypeDef* UARTx, uint8_t *Data,...)
{
    const char *s;
    int d;
    char buf[16];
    va_list ap;
    va_start(ap,Data);
    while(*Data != 0)           //判断是否到达字符串结束符
    {
        if(*Data == 0x5c)       //'\'
        {
            switch(*++Data)
            {
                case 'r':       //回车符
                    UART_SendData(UARTx, 0x0d);
                    Data++;
                    break;
                
                case 'n':       //换行符
                    UART_SendData(UARTx, 0x0a);
                    Data++;
                    break;
                default:
                    Data++;
                    break;
            }
        }
        else if(*Data == '%')
        {
            switch(*++Data)
            {
                case 's':       //字符串
                    s = va_arg(ap, const char *);
                    for(; *s; s++)
                    {
                        UART_SendData(UARTx, *s);
                        while(UART_GetFlagStatus(UARTx, UART_FLAG_TC) == RESET); 
                    }
                    Data++;
                    break;
                case 'd':       //十进制
                    d = va_arg(ap, int);
                    itoa(d, buf, 10);
                    for(s=buf; *s; s++)
                    {
                        UART_SendData(UARTx, *s);
                        while(UART_GetFlagStatus(UARTx, UART_FLAG_TC) == RESET);
                    }
                    Data++;
                    break;
                default:
                    Data++;
                    break;
            }
        }
        else
        {
            UART_SendData(UARTx, *Data++);
        }
        while(UART_GetFlagStatus(UARTx, UART_FLAG_TC) == RESET);
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


