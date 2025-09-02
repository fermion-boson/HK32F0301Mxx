/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    hk32f0301mxxc_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************

  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "hk32f0301mxxc_it.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

extern uint8_t TxBuffer[];
extern uint8_t RxBuffer[];
extern __IO uint8_t Rx_Idx;
extern __IO uint8_t Tx_Idx;

extern __IO uint8_t CmdTransmitted;
extern __IO uint8_t CmdReceived;
extern __IO uint8_t CmdStatus;

__IO uint8_t Counter = 0x00;
extern __IO uint32_t TimeOut;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* Decrement the timeout value */
  if (TimeOut != 0x0)
  {
    TimeOut--;
  }
    
  if (Counter < 10)
  {
    Counter++;
  }
  else
  {
    Counter = 0x00;
  }
}


/**
  * @brief  This function handles SPI interrupt request.
  * @param  None
  * @retval None
  */
void SPI1_IRQHandler(void)
{

#if defined (SPI_SLAVE)  
  
  /* SPI in Slave Tramitter mode--------------------------------------- */
  if (SPI_GetITStatus(SPI_SEL, SPI_IT_TXE) == SET)
  {
    SPI_SendData8(SPI_SEL, TxBuffer[Tx_Idx++]);
    if (Tx_Idx == DATA_SIZE)
    {
      /* Disable the Tx buffer empty interrupt */
      SPI_ITConfig(SPI_SEL, SPI_IT_TXE, DISABLE);
    }
  }
  
  /* SPI in Slave Receiver mode--------------------------------------- */
  if (SPI_GetITStatus(SPI_SEL, SPI_IT_RXNE) == SET)
  {
    if (CmdReceived == 0x00)
    {
      CmdReceived = SPI_ReceiveData8(SPI_SEL);
      CmdStatus = 0x01;
//      printf("Slave receive data : 0x%02x \r\n",CmdReceived);
    }
    else
    {
      RxBuffer[Rx_Idx++] = SPI_ReceiveData8(SPI_SEL);
//      printf("Slave receive data : 0x%02x \r\n",RxBuffer[Rx_Idx]);
    }
  }
  
  /* SPI Error interrupt--------------------------------------- */
  if (SPI_GetITStatus(SPI_SEL, SPI_IT_OVR) == SET)
  {
    SPI_ReceiveData8(SPI_SEL);
    SPI_GetITStatus(SPI_SEL, SPI_IT_OVR);
    printf("Receive data over!\r\n");
  }
  
#endif /* SPI_SLAVE*/
 
#if defined (SPI_MASTER)
  
  /* SPI in Master Tramitter mode--------------------------------------- */
  if (SPI_GetITStatus(SPI_SEL, SPI_IT_TXE) == SET)
  {
    if (CmdStatus == 0x00)
    {
      SPI_SendData8(SPI_SEL, CmdTransmitted);
      CmdStatus = 0x01;
    }
    else
    {
      SPI_SendData8(SPI_SEL, (TxBuffer[Tx_Idx++] + 0x00));
      if (Tx_Idx == DATA_SIZE)
      {
        /* Disable the Tx buffer empty interrupt */
        SPI_ITConfig(SPI_SEL, SPI_IT_TXE, DISABLE);
      }
    }
  }
  
  /* SPI in Master Receiver mode--------------------------------------- */
  if (SPI_GetITStatus(SPI_SEL, SPI_IT_RXNE) == SET)
  {
    if (CmdReceived == 0x00)
    {
      CmdReceived = SPI_ReceiveData8(SPI_SEL);
      Rx_Idx = 0x00;
//      printf("Master receive data : 0x%02x \r\n",CmdReceived);
    }
    else
    {
      RxBuffer[Rx_Idx++] = SPI_ReceiveData8(SPI_SEL);
//      printf("Master receive data : 0x%02x \r\n",RxBuffer[Rx_Idx]);
    }
  }
  
  /* SPI Error interrupt--------------------------------------- */
  if (SPI_GetITStatus(SPI_SEL, SPI_IT_OVR) == SET)
  {
    SPI_ReceiveData8(SPI_SEL);
    SPI_GetITStatus(SPI_SEL, SPI_IT_OVR);
    printf("Receive data over!\r\n");
  }
  
#endif /* SPI_MASTER*/
}

/**
  * @}
  */

/**
  * @}
  */
 
