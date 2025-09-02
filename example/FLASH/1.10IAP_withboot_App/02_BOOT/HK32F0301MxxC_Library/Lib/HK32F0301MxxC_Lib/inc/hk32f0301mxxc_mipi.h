/********************************************************************
* @Copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd 
* @File name: hk32f0301mxxc_mipi.h
* @brief  : this file is a include file that the application programmer
*           is using in the hk32f0301mxxc_mipi.c. 
* @Author : AE Team  
********************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HK32F0301MXXC_MIPI_H
#define __HK32F0301MXXC_MIPI_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hk32f0301mxxc.h"
 
/** @addtogroup MIPI
  * @{
  */ 
  
/** @defgroup MIPI_Exported_Constants MIPI_Exported_Constants
	* @{
	*/ 

/* Exported types ------------------------------------------------------------*/
/** 
  * @brief  MIPI Init structure definition
  * @note   This sturcture is used with MIPI module.
  */

typedef struct
{
  uint16_t IOSEL_P;         			/*!< Specifies the Mipi peripheral DP pin.
                                       This parameter can be a number between 0x00 and 0x1F */

  uint16_t IOSEL_N;       				/*!< Specifies the Mipi peripheral DN pin.
                                       This parameter can be a number between 0x00 and 0x1F */

  uint32_t TX_Length;	            /*!< Specifies the MIPI peripheral Tx data bits counter.
                                       This parameter must be a number between 0x00 and 0xFF.  */ 

  uint8_t TX_RATE;							  /*!< specifies the MIPI peripheral Tx data rate refer AHB-Clock.
                                       This parameter can be a value of @ref MIPI_TxRate_Sel.  */
	
} MIPI_InitTypeDef;       

/* Exported constants --------------------------------------------------------*/

/** @defgroup MIPI_IOSEL MIPI IOSEL 
  * @{
  */ 
#define MIPI_IOSEL_PA0              0
#define MIPI_IOSEL_PA1              1
#define MIPI_IOSEL_PA2              2
#define MIPI_IOSEL_PA3              3
#define MIPI_IOSEL_PA4              4
#define MIPI_IOSEL_PA5              5
#define MIPI_IOSEL_PA6              6
#define MIPI_IOSEL_PA7              7
#define MIPI_IOSEL_PB0              8
#define MIPI_IOSEL_PB1              9
#define MIPI_IOSEL_PB2              10
#define MIPI_IOSEL_PB3              11
#define MIPI_IOSEL_PB4              12
#define MIPI_IOSEL_PB5              13
#define MIPI_IOSEL_PB6              14
#define MIPI_IOSEL_PB7              15
#define MIPI_IOSEL_PC0              16
#define MIPI_IOSEL_PC1              17
#define MIPI_IOSEL_PC2              18
#define MIPI_IOSEL_PC3              19
#define MIPI_IOSEL_PC4              20
#define MIPI_IOSEL_PC5              21
#define MIPI_IOSEL_PC6              22
#define MIPI_IOSEL_PC7              23
#define MIPI_IOSEL_PD0              24
#define MIPI_IOSEL_PD1              25
#define MIPI_IOSEL_PD2              26
#define MIPI_IOSEL_PD3              27
#define MIPI_IOSEL_PD4              28
#define MIPI_IOSEL_PD5              29
#define MIPI_IOSEL_PD6              30
#define MIPI_IOSEL_PD7              31

#define IS_MIPI_IOSEL(IOSEL) ((IOSEL) <= 31)
/**
  * @}
  */
	

/** @defgroup MIPI_interrupt_source MIPI interrupt source
  * @{
  */ 
#define MIPI_IT_TX_EMPTY              ((uint32_t)0x00000010)
#define MIPI_IT_RX_DONE               ((uint32_t)0x00000008)

#define IS_MIPI_ITSource(Source) ((((Source) & (uint32_t)0xFFFFFFE7) == 0) && ((Source) != 0))
/**
	* @}
	*/


/** @defgroup MIPI_TxRate_Sel MIPI TxRate Select
  * @{
  */ 
#define MIPI_TXRATE_SEL_1              ((uint32_t)0x00000000)
#define MIPI_TXRATE_SEL_2              ((uint32_t)0x00000002)
#define MIPI_TXRATE_SEL_3              ((uint32_t)0x00000004)
#define MIPI_TXRATE_SEL_4              ((uint32_t)0x00000006)

#define IS_MIPI_TXRATE_SEL(TXRATE) 		 (((TXRATE) == MIPI_TXRATE_SEL_1) || \
																				((TXRATE) == MIPI_TXRATE_SEL_2) || \
																				((TXRATE) == MIPI_TXRATE_SEL_3) || \
																				((TXRATE) == MIPI_TXRATE_SEL_4))
/**
	* @}
	*/

/** @defgroup MIPI_Flag MIPI Flag
  * @{
  */ 
#define MIPI_FLAG_STOP                    ((uint32_t)0x00000001)
#define MIPI_FLAG_BTA	                    ((uint32_t)0x00000002)
#define MIPI_FLAG_RX	                    ((uint32_t)0x00000004)
#define MIPI_FLAG_TX	                    ((uint32_t)0x00000008)
#define MIPI_FLAG_RX_DONE                 ((uint32_t)0x00000010)
#define MIPI_FLAG_TXD_EMPTY               ((uint32_t)0x00000020)

#define IS_MIPI_GET_FLAG(MIPI_FLAG) 		 (((MIPI_FLAG) == MIPI_FLAG_STOP ) || \
																					((MIPI_FLAG) == MIPI_FLAG_BTA ) || \
																					((MIPI_FLAG) == MIPI_FLAG_RX	) || \
																					((MIPI_FLAG) == MIPI_FLAG_TX ) || \
																					((MIPI_FLAG) == MIPI_FLAG_RX_DONE ) || \
																					((MIPI_FLAG) == MIPI_FLAG_TXD_EMPTY ))
/**
	* @}
	*/

/**
	* @}
	*/
	
/**
	* @}
	*/
  
 
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/ 
 
 
void MIPI_DeInit(void);
void MIPI_Init(MIPI_InitTypeDef *MIPI_InitStruct);
void MIPI_StructInit(MIPI_InitTypeDef* MIPI_InitStruct);
void MIPI_IOSELP(uint32_t IOSEL);
void MIPI_IOSELN(uint32_t IOSEL);
void MIPI_Set_TxBits(uint32_t BitCounter);
void MIPI_ITConfig(uint32_t MIPI_IT,FunctionalState NewState);
void MIPI_SetTxRate(uint32_t TxRate);
void MIPI_Cmd(FunctionalState NewState);
uint8_t MIPI_Get_TX_RX_Counter(void);
FlagStatus MIPI_GetFlagStatus(uint32_t MIPI_FLAG);
void MIPI_SendData(uint32_t MIPI_TxData);
uint32_t MIPI_ReceiveData(void);
uint32_t MIPI_Generate_Data(uint8_t data);
void MIPI_Stop_To_ESC(uint8_t ext_time);
void MIPI_Exit_ESC(void);
void MIPI_Generate_BTA(uint32_t right_shift);

#ifdef __cplusplus
}
#endif





#endif /*__HK32F030MXMXXC_MIPI_H */



