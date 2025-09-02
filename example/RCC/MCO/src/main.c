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
void UART_Configuration(void);
void RCC_Configuration(void);
void GPIO_Configuration(void);
void GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
int fputc(int ch, FILE *f);

static void Delay(__IO uint32_t TCounter)
{
	for(; TCounter != 0; TCounter--);
}

void RCC_Configuration(void)
{	
    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOD, ENABLE );
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOB, ENABLE );
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE );
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOC, ENABLE );
	
	RCC_APBPeriph2ClockCmd( RCC_APBPeriph2_UART1, ENABLE );
		
}

/****************************************
PA3---TX
PD6---RX
PA0---NRST
PD5---SWDIO
PB5---SWDCLK
PA1/PD7/PB5/PC5--EXT
PD4---MCO
PC3---Toggle
*****************************************/
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //PA3，TX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_2;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_1);	

    //PD6，RX
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_1);

    //Toggle IO PC3
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //MCO  PD4
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_2;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource4,GPIO_AF_5);	
}

void UART_Configuration(void)
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

void GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  /* Check the parameters */
  assert_param(IS_GPIO_PIN(GPIO_Pin));

  GPIOx->ODR ^= GPIO_Pin;
	
}

int main(void)
  /* Infinite loop */
{
	RCC_Configuration();
	
	GPIO_Configuration();
	
    //HSI 分频测试
	RCC_MCOConfig(RCC_MCOSource_SYSCLK, RCC_MCOPrescaler_1);
    
    //切换成MCO--HSI
    RCC_MCOConfig(RCC_MCOSource_HSI, RCC_MCOPrescaler_8);
    
    //打开LSI MCO--LSI
    __IO uint32_t StartUpCounter = 0, LSIStatus = 0;
	
	/* Enable LSI */
    RCC->CSR |= RCC_CSR_LSION;

	/* Wait till LSI is ready and if Time out is reached exit */
	do{
		LSIStatus = RCC->CSR & RCC_CSR_LSIRDY;
		StartUpCounter++;  
	} while((LSIStatus == 0) && (StartUpCounter != STARTUP_TIMEOUT));

    RCC_MCOConfig(RCC_MCOSource_LSI, RCC_MCOPrescaler_1);
    
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


