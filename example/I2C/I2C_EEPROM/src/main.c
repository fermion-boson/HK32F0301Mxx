#include "main.h"
#include "hk32f0301mxxc.h"
#include "iic.h"

/* Data sent */
uint8_t SendBuf[8] = {0x02,0x04,0x06,0x08,0x0a,0x0c,0x0e,0x10};

/* Received data */
uint8_t ReceiveBuf[24] = {0};

/**
  * @brief  Delay function.
  * @retval None
  */
static void Delay(uint32_t i)
{
	while(i--);
}

/**
  * @brief  Configure uart.
  * @retval None
  */
void UART_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;

	/*  Enables clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA , ENABLE);
    RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_UART1, ENABLE);

    /* Initializes the GPIOx peripheral */
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    /* selects the pin to used as Alternate function */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);

    /* Initializes the UARTx peripheral */
    UART_InitStructure.UART_BaudRate=115200;
    UART_InitStructure.UART_WordLength=UART_WordLength_8b;
    UART_InitStructure.UART_StopBits=UART_StopBits_1;
    UART_InitStructure.UART_Parity=UART_Parity_No;
    UART_InitStructure.UART_Mode=UART_Mode_Tx;
    UART_Init(UART1, &UART_InitStructure);

    /* Enables UARTx peripheral */
    UART_Cmd(UART1, ENABLE);
}

/**
  * @brief  Main program
  * @retval None
  */
int main()
{
	/* Configure uart */
	UART_Config();
	
	/* Configure I2C */
	I2C_Config();
    
	I2C_Write(0xa0,0x00,SendBuf,8);
    /* Waiting for data to be written */
    Delay(8000);
    
	I2C_Write(0xa0,0x08,SendBuf,8);
    /* Waiting for data to be written */
	Delay(8000);
    
	I2C_Write(0xa0,0x10,SendBuf,8);
	/* Waiting for data to be written */
	Delay(8000);
    
    I2C_Read(0xa0,0,ReceiveBuf,24);
	for(uint8_t i = 0; i < 24; i++)
	{
		printf("Receive data[%d] = %x\n",i,ReceiveBuf[i]);
	}
	
	while(1)
	{			
        ;
	}
}


/* 重定向c库函数printf到串口，重定向后可使用printf函数 */
PUTCHAR_PROTOTYPE
{
    /* Transmits single data through the UARTx peripheral */
    UART_SendData(UART1, (uint8_t) ch);
	
    /* wait for sending */
    while(UART_GetFlagStatus(UART1, UART_FLAG_TXE)==RESET);

    return(ch);
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



