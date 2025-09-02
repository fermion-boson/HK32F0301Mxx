/********************************************************************
* @Copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd 
* @File name: hk32f0301mxxc_mipi.c
* @brief  : This driver provides functions to configure and program the MIPI of all hk32f0301mxxc devices.
* @Author : AE Team  
* @Version: V1.0.0/2022-09-09 
* @log  :
*********************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "hk32f0301mxxc_mipi.h"
#include "hk32f0301mxxc_rcc.h"

/** @addtogroup HK32F0301MxxC_StdPeriph_Driver
  * @{
  */

/** @defgroup MIPI  MIPI
  * @brief MIPI driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup MIPI_Private_Functions MIPI_Private_Functions
  * @{
  */
	
	
/**
  * @brief  Deinitializes the MIPI peripheral registers to their default reset 
  *         values.
  * @note  	None
  * @retval None
  */
void MIPI_DeInit(void)
{
	RCC_APBPeriph1ResetCmd(RCC_APBPeriph1_IOMUX, ENABLE);
	RCC_APBPeriph1ResetCmd(RCC_APBPeriph1_IOMUX, DISABLE);
}

/**
  * @brief  Initializes the MIPI peripheral according to 
  *         the specified parameters in the MIPI_InitTypeDef.
  * @param  MIPI_InitStruct: pointer to a MIPI_InitTypeDef
  *         structure that contains the configuration information for
  *         the specified MIPI peripheral.
  * @retval None
  */
void MIPI_Init(MIPI_InitTypeDef *MIPI_InitStruct)
{
  /* Check the parameters */
  assert_param(IS_MIPI_IOSEL(MIPI_InitStruct->IOSEL_N));
  assert_param(IS_MIPI_IOSEL(MIPI_InitStruct->IOSEL_P));
  assert_param(IS_MIPI_TXRATE_SEL(MIPI_InitStruct->TX_RATE));
	
	/* Configures the IOSEL_P, IOSEL_N, TX_Length and TX_RATE */
  MIPI->CR &= ~(MIPI_CR_IOSEL_P | MIPI_CR_IOSEL_N  | MIPI_CR_TX_LEN  | MIPI_CR_TX_RATE );

  MIPI->CR |= (uint32_t)((MIPI_InitStruct->IOSEL_P << MIPI_CR_IOSEL_P_Pos)| \
												 (MIPI_InitStruct->IOSEL_N << MIPI_CR_IOSEL_N_Pos)|	\
												 (MIPI_InitStruct->TX_Length << MIPI_CR_TX_LEN_Pos)| \
												 (MIPI_InitStruct->TX_RATE ));
}

/**
  * @brief  Fills each MIPI_InitStruct member with its default value.
  * @param  MIPI_InitStruct: pointer to a MIPI_InitTypeDef structure which will be initialized.
  * @retval None
  */
void MIPI_StructInit(MIPI_InitTypeDef* MIPI_InitStruct)
{
  /* Set the default configuration */
	MIPI_InitStruct->IOSEL_P = MIPI_IOSEL_PA0;
	MIPI_InitStruct->IOSEL_N = MIPI_IOSEL_PA1;
	MIPI_InitStruct->TX_Length = 0xff;
	MIPI_InitStruct->TX_RATE = MIPI_TXRATE_SEL_4;
}

/**
  * @brief  Select Mipi peripheral DP pin
  * @note  	None
  * @param  IOSEL: specifies the MIPI peripheral DP pin.
	* 					This parameter must be a number between 0x00 and 0x1F .
  * @retval None
  */
void MIPI_IOSELP(uint32_t IOSEL)
{
  assert_param(IS_MIPI_IOSEL(IOSEL));
	
	MIPI->CR &= ~(uint32_t)(MIPI_CR_IOSEL_P);
	MIPI->CR |= IOSEL << MIPI_CR_IOSEL_P_Pos;
}

/**
  * @brief  Select Mipi peripheral DN pin
  * @note  	None
  * @param  IOSEL: specifies the MIPI peripheral DP pin.
	* 					This parameter must be a number between 0x00 and 0x1F .
  * @retval None
  */
void MIPI_IOSELN(uint32_t IOSEL)
{
  assert_param(IS_MIPI_IOSEL(IOSEL));
	
	MIPI->CR &= ~(uint32_t)(MIPI_CR_IOSEL_N);
	MIPI->CR |= IOSEL << MIPI_CR_IOSEL_N_Pos;
}

/**
  * @brief  Specifies the MIPI peripheral Tx data bits counter.
  * @note  	None
  * @param  BitCounter: specifies the MIPI peripheral Tx data bits counter.
	* 					This parameter must be a number between 0x00 and 0xFF .
  * @retval None
  */
void MIPI_Set_TxBits(uint32_t BitCounter)
{
	MIPI->CR &= ~(uint32_t)(MIPI_CR_TX_LEN);
	MIPI->CR |= BitCounter << MIPI_CR_TX_LEN_Pos;
}

/**
  * @brief  Specifies the MIPI peripheral interrupt sources.
  * @note  	None
  * @param  MIPI_IT: specifies the MIPI peripheral interrupt sources.
  *          This parameter can be any combination of the following values:
  *            @arg MIPI_IT_TX_EMPTY 
  *            @arg MIPI_IT_RX_DONE  
  * @param  NewState: new state of the MIPI interrupts.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void MIPI_ITConfig(uint32_t MIPI_IT,FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_MIPI_ITSource(MIPI_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
	
  if (NewState != DISABLE)
  {
    /* Enable the Interrupt sources */
		MIPI->CR |= MIPI_IT;
  }
  else
  {
    /* Disable the Interrupt sources */
		MIPI->CR &= (uint32_t)~MIPI_IT;
  }
}

/**
  * @brief  Specifies the MIPI peripheral Tx data rate.
  * @note  	None
  * @param  TxRate: specifies the MIPI peripheral Tx data rate.
  *          This parameter can be one of the following values:
  *            @arg MIPI_TXRATE_SEL_1  
  *            @arg MIPI_TXRATE_SEL_2  
  *            @arg MIPI_TXRATE_SEL_3 
  *            @arg MIPI_TXRATE_SEL_4 
	* @retval None
  */
void MIPI_SetTxRate(uint32_t TxRate)
{
  /* Check the parameters */
  assert_param(IS_MIPI_TXRATE_SEL(TxRate));
	
	MIPI->CR &= ~(uint32_t)(MIPI_CR_TX_RATE);
	MIPI->CR |= TxRate;
}


/**
  * @brief  Enable the MIPI peripheral .
  * @note  	None
  * @param  NewState: new state of the MIPI enable.
  *          This parameter can be: ENABLE or DISABLE.
	* @retval None
  */
void MIPI_Cmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
	
	if (NewState != DISABLE)
  {
    /* Enable the MIPI peripheral*/
		MIPI->CR |= MIPI_CR_EN;
  }
  else
  {
    /* Disable the MIPI peripheral */
		MIPI->CR &= (uint32_t)~MIPI_CR_EN;
  }
}


/**
  * @brief  Get MIPI Tx remained bits counter or Rx received bits counter.
  * @note  	None
	* @retval The Value of bits counter.
  */
uint8_t MIPI_Get_TX_RX_Counter(void)
{
	uint8_t bitscounter=0;
	bitscounter = (uint8_t)(MIPI->SR >> MIPI_SR_RTX_CNT_Pos) ;
	
	return(bitscounter);
}


/**
  * @brief  Checks whether the specified MIPI flag is set or not..
  * @param  MIPI_FLAG: specifies the flag to check.
  *          This parameter can be one of the following values:
  *            @arg MIPI_FLAG_STOP      
  *            @arg MIPI_FLAG_BTA	      
  *            @arg MIPI_FLAG_RX	     
  *            @arg MIPI_FLAG_TX	     
  *            @arg MIPI_FLAG_RX_DONE  
  *            @arg MIPI_FLAG_TXD_EMPTY
  * @note  	None
	* @retval The new state of TIM_FLAG (SET or RESET)..
  */
FlagStatus MIPI_GetFlagStatus(uint32_t MIPI_FLAG)
{
  FlagStatus bitstatus = RESET; 
   
  /* Check the parameters */
  assert_param(IS_MIPI_GET_FLAG(MIPI_FLAG));
  
  if ((MIPI->SR & MIPI_FLAG) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
	
	return bitstatus;
}

/**
  * @brief  MIPI send data out once MIPI get BTA and turn into Tx mode.
  * @param  MIPI_TxData: specifies the data to send.
  *          This parameter can be any 32bit values that includes Dp and Dn levels.
  * @note  	The Txdata write into TX_DATA register when TX_DATA register is empty.
  * @note  	The Txdata MSB is send fisrt.
	* @retval None.
  */
void MIPI_SendData(uint32_t MIPI_TxData)
{
	MIPI->TX_DATA = MIPI_TxData;
}


/**
  * @brief  MIPI receive data in Rx mode.
  * @note  	The Rxdata LSB is last received bit.
	* @retval The Value of RX_DATA.
  */
uint32_t MIPI_ReceiveData(void)
{
	return MIPI->RX_DATA ;
}


/**
  * @brief  generate a mipi data,its format is spece-one-hot.
	* @param  data: data to send,including LPDT/DI/DATA/ECC
  * @retval used for mipi moudle's MIPI_TX_DATA
  */
uint32_t MIPI_Generate_Data(uint8_t data)
{
	uint16_t Seq_Dp=0,Seq_Dn=0;
	/*Generate a differential signal for MIPI Tx*/
	for(int i=0;i<8;i++)
	{
		if(data & 0x80)
			Seq_Dp=1<<(16-2*i);
		else
			Seq_Dn=1<<(16-2*i);
		data <<=1;
	}
	return (Seq_Dp<<16)|Seq_Dn;
}

/**
  * @brief  generate a ESC from stop(idle) mode.
	* @param  ext_time: set extra time to get a longer idle state
  * @retval None
  */
void MIPI_Stop_To_ESC(uint8_t ext_time)
{
	while(ext_time--)
	{
		MIPI_SendData(0xffffffff);
		while(MIPI_GetFlagStatus(MIPI_FLAG_TXD_EMPTY)==RESET);
	}
	/*Generate a ESC code	LP11--LP10--LP00--LP01--LP00*/
	MIPI->TX_DATA=0x7ff83ff2;
}

/**
  * @brief  generate a exit from stop mode.
	* @note   None
  * @retval None
  */
void MIPI_Exit_ESC(void)
{
	/*generate a exit from stop mode: LP10--LP11*/
	while(MIPI_GetFlagStatus(MIPI_FLAG_TXD_EMPTY)==RESET);
	MIPI_SendData(0xffff7fff);
}

/**
  * @brief  generate a BTA.
	* @param  right_shift: set the BTA double drive time shift
  * @retval None
  */
void MIPI_Generate_BTA(uint32_t right_shift)
{
	uint32_t temp;
	
	/*generate a ESC code	LP11--LP10--LP00--LP10--LP00*/	
	uint16_t Dp_BTA=0xD000,Dn_BTA=0x8000;
	Dp_BTA = (Dp_BTA>>right_shift)|(0xffff<<(16-right_shift));
	Dn_BTA = (Dn_BTA>>right_shift)|(0xffff<<(16-right_shift));
	/*Insert a stop here*/
	temp = Dp_BTA<<16 | Dn_BTA |0x30001 ;	
	
	while(MIPI_GetFlagStatus(MIPI_FLAG_TXD_EMPTY)==RESET);
	MIPI_SendData(temp);
}

/**
  * @}
  */
/**
  * @}
  */
/**
  * @}
  */
 
