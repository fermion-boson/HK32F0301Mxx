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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SPI_InitTypeDef  SPI_InitStructure;


uint8_t TxBuffer[10] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x00};

uint8_t RxBuffer [RXBUFFERSIZE];

__IO uint8_t Rx_Idx = 0x00;
__IO uint8_t Tx_Idx = 0x00;

__IO uint8_t CmdTransmitted = 0x00;
__IO uint8_t CmdReceived = 0x00;
__IO uint8_t CmdStatus = 0x00;
__IO uint32_t TimeOut = 0x0;

/* Private function prototypes -----------------------------------------------*/
static void SPI_Config(void);
static void SysTickConfig(void);
static void TimeOut_UserCallback(void);
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength, uint8_t DataMask);


/* Private functions ---------------------------------------------------------*/

int main(void)
  /* Infinite loop */
{
  
  /* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  UART_InitTypeDef UART_InitStructure;
  UART_InitStructure.UART_BaudRate = 115200;
  UART_InitStructure.UART_WordLength = UART_WordLength_8b;
  UART_InitStructure.UART_StopBits = UART_StopBits_1;
  UART_InitStructure.UART_Parity = UART_Parity_No;
  UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;

  UART_COMInit(&UART_InitStructure);  
  printf("Test SPI ~~\r\n");
  
/* SPI configuration ------------------------------------------------------*/
  SPI_Config();
  
  /* SysTick configuration ---------------------------------------------------*/
  SysTickConfig();
  
  /* Master board configuration ------------------------------------------------*/
#ifdef SPI_MASTER
  printf("For SPI Master£º\r\n");
  
  /* Initializes the SPI communication */
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_Init(SPI_SEL, &SPI_InitStructure);
  
  /* Initialize the FIFO threshold */
  SPI_RxFIFOThresholdConfig(SPI_SEL, SPI_RxFIFOThreshold_QF);
  
  /* Enable the Rx buffer not empty interrupt */
  SPI_ITConfig(SPI_SEL, SPI_IT_RXNE, ENABLE);
  /* Enable the SPI Error interrupt */
  SPI_ITConfig(SPI_SEL, SPI_IT_ERR, ENABLE);
  /* Data transfer is performed in the SPI interrupt routine */
  
  /* Enable the SPI peripheral */
  SPI_Cmd(SPI_SEL, ENABLE);
  
  while (1)
  {
    CmdTransmitted = 0x00;
    CmdReceived = 0x00;
    CmdStatus = 0x00;
    Tx_Idx = 0x00;
    Rx_Idx = 0x00; 
    
    CmdTransmitted = 0x55;
    
    if (CmdTransmitted != 0x00)
    {    
      /* Enable the Tx buffer empty interrupt */
      SPI_ITConfig(SPI_SEL, SPI_IT_TXE, ENABLE);
      
      /* Wait until end of data transfer or time out*/
      TimeOut = USER_TIMEOUT;
      while ((Rx_Idx < DATA_SIZE)&&(TimeOut != 0x00))
      {}
      if(TimeOut == 0)
      {
        TimeOut_UserCallback();
      }

      for(uint8_t j=0; j<RXBUFFERSIZE; j++)
      {
          printf("Master receive data : 0x%02x \r\n",RxBuffer[j]);
      }
    }
    
    /* Waiting until TX FIFO is empty */
    while (SPI_GetTransmissionFIFOStatus(SPI_SEL) != SPI_TransmissionFIFOStatus_Empty)
    {}
    
    /* Wait busy flag */
    while(SPI_GetFlagStatus(SPI_SEL, SPI_FLAG_BSY) == SET)
    {}
    
    /* Waiting until RX FIFO is empty */
    while (SPI_GetReceptionFIFOStatus(SPI_SEL) != SPI_ReceptionFIFOStatus_Empty)
    {}
    
    if ((Buffercmp(TxBuffer, RxBuffer, DATA_SIZE, SPI_DATAMASK) == PASSED) && (CmdReceived == CMD_ACK))
    {

        printf("## Test OK ##\r\n");
    }
    else
    {
        printf("!! Test ERR !!\r\n");
    }

    while(1);
  }
#endif /* SPI_MASTER */
  
  /* Slave board configuration ----------------------------------------------*/
#ifdef SPI_SLAVE
  printf("For SPI Slave\r\n");

  /* Initializes the SPI communication */
  SPI_DeInit(SPI_SEL);
  SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
  SPI_Init(SPI_SEL, &SPI_InitStructure);
  
  /* Initialize the FIFO threshold */
  SPI_RxFIFOThresholdConfig(SPI_SEL, SPI_RxFIFOThreshold_QF);
  
  /* Enable the Rx buffer not empty interrupt */
  SPI_ITConfig(SPI_SEL, SPI_IT_RXNE, ENABLE);
  
  /* Enable the SPI Error interrupt */
  SPI_ITConfig(SPI_SEL, SPI_IT_ERR, ENABLE);
  
  /* Enable the SPI peripheral */
  SPI_Cmd(SPI_SEL, ENABLE);
  
  /* Infinite Loop */
  while (1)
  {     
    CmdStatus = 0x00;
    CmdReceived = 0x00;
    Rx_Idx = 0x00;
    Tx_Idx = 0x00;
    
    /* Write the first data in SPI shift register before enabling the interrupt
    this data will be transmitted when the Slave receive the generated clock
    by the Master */    
    /* Enable the Tx buffer empty interrupt */
    SPI_SendData8(SPI_SEL, CMD_ACK);
    SPI_ITConfig(SPI_SEL, SPI_IT_TXE, ENABLE);
    
    /* Waiting Transaction code Byte */
    while (CmdStatus == 0x00)
    {}
    
    TimeOut = USER_TIMEOUT;
    while ((Rx_Idx < DATA_SIZE)&&(TimeOut != 0x00))
    {}
    if(TimeOut == 0)
    {
      TimeOut_UserCallback();
    }

    for(uint8_t j=0; j<RXBUFFERSIZE; j++)
    {
        printf("Slave receive data : 0x%02x \r\n",RxBuffer[j]);
    }
    
    if(CmdReceived == 0x55)
    {
      if (Buffercmp(TxBuffer, RxBuffer, DATA_SIZE, SPI_DATAMASK) != FAILED) 
      {
          printf("## Test OK ##\r\n");
      }
      else
      {
          printf("!! Test ERR !!\r\n");
      }
    }
    
    /* Waiting until TX FIFO is empty */
    while (SPI_GetTransmissionFIFOStatus(SPI_SEL) != SPI_TransmissionFIFOStatus_Empty)
    {}    
    
    /* Wait busy flag */
    while(SPI_GetFlagStatus(SPI_SEL, SPI_FLAG_BSY) == SET)
    {}
    
    /* Disable the Tx buffer empty interrupt */
    SPI_ITConfig(SPI_SEL, SPI_IT_TXE, DISABLE);
    
    /* Waiting until RX FIFO is empty */
    while (SPI_GetReceptionFIFOStatus(SPI_SEL) != SPI_ReceptionFIFOStatus_Empty)
    {} 
    
  }
#endif /* SPI_SLAVE */

}

/**
* @brief  Basic management of the timeout situation.
* @param  None.
* @retval None.
*/
static void TimeOut_UserCallback(void)
{
  /* User can add his own implementation to manage TimeOut Communication failure */
  /* Block communication and all processes */
  while (1)
  {   
  }
}

/**
  * @brief  Configures the SPI Peripheral.
  * @param  None
  * @retval None
  */
static void SPI_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Enable the SPI periph */
  RCC_APBPeriph2ClockCmd(SPI_CLK, ENABLE);
  
  /* Enable SCK, MOSI, MISO and NSS GPIO clocks */
  RCC_AHBPeriphClockCmd(SPI_SCK_GPIO_CLK | SPI_MISO_GPIO_CLK | SPI_MOSI_GPIO_CLK | SPI_NSS_GPIO_CLK, ENABLE);
  
  GPIO_PinAFConfig(SPI_SCK_GPIO_PORT, SPI_SCK_SOURCE, SPI_SCK_AF);
  GPIO_PinAFConfig(SPI_MOSI_GPIO_PORT, SPI_MOSI_SOURCE, SPI_MOSI_AF);
  GPIO_PinAFConfig(SPI_MISO_GPIO_PORT, SPI_MISO_SOURCE, SPI_MISO_AF);
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_2;

  /* SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = SPI_SCK_PIN;
  GPIO_Init(SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* SPI  MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  SPI_MOSI_PIN;
  GPIO_Init(SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* SPI MISO pin configuration */
  GPIO_InitStructure.GPIO_Pin = SPI_MISO_PIN;
  GPIO_Init(SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
  
  /* SPI configuration -------------------------------------------------------*/
  SPI_DeInit(SPI_SEL);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_DataSize = SPI_DATASIZE;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
   
  /* Configure the SPI interrupt priority */
  NVIC_InitStructure.NVIC_IRQChannel = SPI_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Configure a SysTick Base time to 10 ms.
  * @param  None
  * @retval None
  */
static void SysTickConfig(void)
{
  /* Setup SysTick Timer for 10ms interrupts  */
  if (SysTick_Config(SystemCoreClock / 100))
  {
    /* Capture error */
    while (1);
  }

  /* Configure the SysTick handler priority */
  NVIC_SetPriority(SysTick_IRQn, 0x0);
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength, uint8_t DataMask)
{
  while (BufferLength--)
  {
    if (((*pBuffer1) & DataMask) != *pBuffer2)
    {
      return FAILED;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;
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


