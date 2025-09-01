/**
  ******************************************************************************
  * @file    hk32f0301m_pwr.h
  * @author  Rakan.Z
  * @version V1.0  
  * @brief   Header file of PWR module
  * @changelist  
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HK32F0301M_PWR_H
#define __HK32F0301M_PWR_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hk32f0301m.h"

/** @defgroup DBP
  * @{
  */

#define PWR_DBP_MSK           ((uint32_t)0x00000100)

/** @defgroup CSR
  * @{
  */
#define PWR_EWUP1_MSK         ((uint32_t)0x00000100)

/** @defgroup Regulator_state_is_STOP_mode 
  * @{
  */

#define PWR_Regulator_LowPower    ((uint32_t)0x00000001)
#define IS_PWR_REGULATOR(REGULATOR) ((REGULATOR) == PWR_Regulator_LowPower)

/** @defgroup PWR_mode_entry 
  * @{
  */

#define PWR_Entry_WFI         ((uint8_t)0x01)
#define PWR_Entry_WFE         ((uint8_t)0x02)
#define IS_PWR_ENTRY(ENTRY) (((ENTRY) == PWR_Entry_WFI) || ((ENTRY) == PWR_Entry_WFE))
 
/** @defgroup PWR_Flag 
  * @{
  */

#define PWR_FLAG_WU               ((uint32_t)0x00000001)
#define PWR_FLAG_SB               ((uint32_t)0x00000002)
#define PWR_FLAG_LDORDY           ((uint32_t)0x00000008)
#define IS_PWR_GET_FLAG(FLAG) (((FLAG) == PWR_FLAG_WU) || ((FLAG) == PWR_FLAG_SB) || \
                               || ((FLAG) == PWR_FLAG_LDORDY))

#define IS_PWR_CLEAR_FLAG(FLAG) (((FLAG) == PWR_FLAG_WU) || ((FLAG) == PWR_FLAG_SB))

/** @defgroup PWR_WUPOL
  * @{
  */
#define PWR_WUPOL_MSK           ((uint32_t)0x00000001)

typedef enum {RISING = 0, FALLING = !RISING} WakePolarity;

/** @defgroup PWR_LDO_VREF
  * @{
  */
#define ADC_VREF_0D8              ((uint16_t)0x0008)  
#define ADC_VREF_LDO              ((uint16_t)0x000C)  
#define VTEST_SET_MASK            ((uint16_t)0x3FFF)
#define IS_PWR_VTEST_SET(VTEST_SET) (((VTEST_SET) == ADC_VREF_0D8) || ((VTEST_SET) == ADC_VREF_LDO))




void PWR_DeInit(void);
void PWR_BackupAccessCmd(FunctionalState NewState);
void PWR_WakeUpPinCmd(FunctionalState NewState);
void PWR_EnterSleepMode(uint8_t PWR_Entry);
void PWR_EnterDeepSleepMode(uint8_t PWR_Entry);
void PWR_EnterStopMode(uint32_t PWR_Regulator, uint8_t PWR_Entry);
void PWR_EnterStandbyMode(void);

FlagStatus PWR_GetFlagStatus(uint32_t PWR_FLAG);
void PWR_ClearFlag(uint32_t PWR_FLAG);

void PWR_WakeUpPinPolarity(WakePolarity eWakePolarity);
void PWR_PowerDownDetect_Cmd(FunctionalState NewState);

void PWR_StandbyMode_BKP0(uint32_t BackupData0);
void PWR_StandbyMode_BKP1(uint32_t BackupData1);

void PWR_StandbyAutoWakeupTimer_Cmd(FunctionalState NewState);
void PWR_SetStandbyWakeupTimer(uint32_t Timercount);

void PWR_SetLDO_RefVolToADC(uint16_t Vref_Set);


#ifdef __cplusplus
}
#endif

#endif

