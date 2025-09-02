/********************************************************************
* @Copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd 
* @Filename: k32f0301mxxc_pwr.h
* @brief  : This file contains all the functions prototypes for the PWR firmware 
*          library 
* @Author : AE Team  
********************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HK32F0301MXXC_PWR_H
#define __HK32F0301MXXC_PWR_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hk32f0301mxxc.h"


/** @addtogroup PWR
  * @{
  */ 
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/** @defgroup PWR_Exported_Constants PWR_Exported_Constants
  * @{
  */ 
  


/** @defgroup Regulator_state_is_STOP_mode Regulator_state_is_STOP_mode
  * @{
  */

#define PWR_Regulator_ON          ((uint32_t)0x00000000)
#define PWR_Regulator_LowPower    ((uint32_t)0x00000001)
#define IS_PWR_REGULATOR(REGULATOR) (((REGULATOR) == PWR_Regulator_LowPower)||((REGULATOR)==PWR_Regulator_ON))

/**
  * @}
  */


/** @defgroup PWR_mode_entry PWR_mode_entry
  * @{
  */

#define PWR_Entry_WFI              ((uint8_t)0x01)
#define PWR_Entry_WFE              ((uint8_t)0x02)
#define PWR_Entry_SLEEPONEXIT      ((uint8_t)0x03)
#define IS_PWR_ENTRY(ENTRY) (((ENTRY) == PWR_Entry_WFI) || ((ENTRY) == PWR_Entry_WFE) ||\
                                   ((ENTRY) == PWR_Entry_SLEEPONEXIT))


/**
  * @}
  */
 
/** @defgroup PWR_Flag PWR_Flag
  * @{
  */

#define PWR_FLAG_LDORDY           ((uint32_t)0x00000008)
#define IS_PWR_GET_FLAG(FLAG) ((FLAG) == PWR_FLAG_LDORDY)

#define IS_PWR_CLEAR_FLAG(FLAG) (((FLAG) == PWR_FLAG_WU) || ((FLAG) == PWR_FLAG_SB))

/**
  * @}
  */


/** @defgroup PWR_VTEST_OUT PWR_VTEST_OUT
  * @{
  */
#define PWR_VTEST_LDO_CORE              ((uint16_t)0x0007)  
#define PWR_VTEST_MBGP              		((uint16_t)0x0006)  
#define PWR_VTEST_LPLDO            			((uint16_t)0x0005)
#define PWR_VTEST_LPBGP           			((uint16_t)0x0004)
#define IS_PWR_VTEST_SEL(VTEST_SEL) (((VTEST_SEL) == PWR_VTEST_LDO_CORE) || ((VTEST_SEL) == PWR_VTEST_MBGP) || \
																		 ((VTEST_SEL) == PWR_VTEST_LPLDO) || ((VTEST_SEL) == PWR_VTEST_LPBGP))

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup PWR_Exported_Functions PWR_Exported_Functions
  * @{
  */
void PWR_DeInit(void);
void PWR_EnterSleepMode(uint8_t PWR_Entry);
void PWR_EnterStopMode(uint32_t PWR_Regulator, uint8_t PWR_Entry);

FlagStatus PWR_GetFlagStatus(uint32_t PWR_FLAG);


void PWR_VTEST_Sel(uint16_t VTEST_Sel);

#ifdef __cplusplus
}
#endif
	
#endif


/**
  * @}
  */

/**
  * @}
  */
