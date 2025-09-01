/**
  ******************************************************************************
  * @file    hk32f0301m_pwr.c
  * @author  Rakan.zhang
  * @version V1.0  
  * @brief   API file of PWR module
  * @changelist  
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "hk32f0301m_pwr.h"
#include "hk32f0301m_rcc.h"
#include "hk32f0301m.h"
/** @defgroup PWR_Private_Defines
  * @{
  */

/* ------------------ PWR registers bit mask ------------------------ */

/* CR register bit mask */
#define CR_DS_MASK               ((uint32_t)0xFFFFFFFC)


/**
  * @brief  Deinitializes the PWR peripheral registers to their default reset values.
  * @param  None
  * @retval None
  */
void PWR_DeInit(void)
{
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_PWR, ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_PWR, DISABLE);
}

/**
  * @brief  Enables or disables access to the  backup registers.
  * @param  NewState: new state of the access to the  backup registers.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void PWR_BackupAccessCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
	if(NewState == ENABLE)
	{
		PWR->CR |= PWR_DBP_MSK;
	}
	else
	{
		PWR->CR &= (uint32_t)~PWR_DBP_MSK;
	}
}



/**
  * @brief  Enters Sleep mode.
  * @param  PWR_Entry: specifies if Sleep mode in entered with WFI or WFE instruction.
  *   This parameter can be one of the following values:
  *     @arg PWR_Entry_WFI: enter Sleep mode with WFI instruction
  *     @arg PWR_Entry_WFE: enter Sleep mode with WFE instruction
  * @retval None
  */
void PWR_EnterSleepMode(uint8_t PWR_Entry)
{
  uint32_t tmpreg = 0;
  /* Check the parameters */
  assert_param(IS_PWR_ENTRY(PWR_Entry));
  
  /* Select the regulator state in Sleep mode ---------------------------------*/
  tmpreg = PWR->CR;
  /* Clear PDDS and LPDS bits */
  tmpreg &= CR_DS_MASK;
  /* Store the new value */
  PWR->CR = tmpreg;

  /* Select STOP mode entry --------------------------------------------------*/
  if(PWR_Entry == PWR_Entry_WFI)
  {   
    /* Request Wait For Interrupt */
    __WFI();
  }
  else
  {
    /* Request Wait For Event */
  	__SEV();
    __WFE();
	  __WFE();
  }  
}

/**
  * @brief  it will config LSI 128K as sysclk
  * @retval None
  * @note this fuction only used in fuction  PWR_EnterDeepSleepMode(uint8_t PWR_Entry)
  */
static void Sysclk_SwitchToLSI(void)
{
	RCC_LSICmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
	
	/* Flash wait state */
	FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
	FLASH->ACR |= (uint32_t)FLASH_Latency_0;	
	
	/* Select LSI as system clock source */
	RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
	RCC->CFGR |= (uint32_t)RCC_CFGR_SW_LSI; 
	/* Wait till LSI is used as system clock source */
	while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != RCC_CFGR_SWS_LSI);
	
	/* HCLK = SYSCLK */
	RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

	/* PCLK = HCLK */
	RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE_DIV1;
	
	// config the Flash Erase and program time
	RCC->CFGR4 |= RCC_RCC_CFGR4_FLITFCLK_PRE;
	RCC->CFGR4 &= ~(((uint32_t)0x0F) << RCC_RCC_CFGR4_FLITFCLK_PRE_Pos);
	
	/* Close HSI */
	RCC_HSICmd(DISABLE);

}

/**
  * @brief  Enters DeepSleep mode. it will config LSI 128K as sysclk
  * @param  PWR_Entry: specifies if Sleep mode in entered with WFI or WFE instruction.
  *   This parameter can be one of the following values:
  *     @arg PWR_Entry_WFI: enter Sleep mode with WFI instruction
  *     @arg PWR_Entry_WFE: enter Sleep mode with WFE instruction
  * @retval None
  */
void PWR_EnterDeepSleepMode(uint8_t PWR_Entry)
{
  /* Check the parameters */
  assert_param(IS_PWR_ENTRY(PWR_Entry));
  /* set sysclk  to LSI */
  Sysclk_SwitchToLSI();
  /* enter sleep mode  */
  PWR_EnterSleepMode(PWR_Entry);
}

/**
  * @brief  Enter Stop mode.
  * @param  PWR_Regulator: specifies the regulator state in STOP mode.
  *   This parameter can be one of the following values:
  *     @arg PWR_Regulator_LowPower: STOP mode with regulator in low power mode
  * @param  PWR_Entry: specifies if STOP mode in entered with WFI or WFE instruction.
  *   This parameter can be one of the following values:
  *     @arg PWR_Entry_WFI: enter STOP mode with WFI instruction
  *     @arg PWR_Entry_WFE: enter STOP mode with WFE instruction
  * @retval None
  */
void PWR_EnterStopMode(uint32_t PWR_Regulator, uint8_t PWR_Entry)
{
  uint32_t tmpreg = 0;
  /* Check the parameters */
  assert_param(IS_PWR_REGULATOR(PWR_Regulator));
  assert_param(IS_PWR_ENTRY(PWR_Entry));
  
  /* Select the regulator state in Stop mode ---------------------------------*/
  tmpreg = PWR->CR;
  /* Clear PDDS and LPDS bits */
  tmpreg &= CR_DS_MASK;
  /* Set LPDS bit according to PWR_Regulator value */
  tmpreg |= PWR_Regulator;
  /* Store the new value */
  PWR->CR = tmpreg;
  /* Set SLEEPDEEP bit of Cortex System Control Register */
  SCB->SCR |= SCB_SCR_SLEEPDEEP;
  
  /* Select Stop mode entry --------------------------------------------------*/
  if(PWR_Entry == PWR_Entry_WFI)
  {   
    /* Request Wait For Interrupt */
    __WFI();
  }
  else
  {
    // wait the AWU is IDE and AWU_BUSY is 0
    while(AWU->SR & 0x00000001){};

    //  detect and clear the AWU_EXTILINE11
    if(EXTI_GetFlagStatus(EXTI_Line11) == SET)
    {
      EXTI_ClearFlag(EXTI_Line11);
    }
    /* Request Wait For Event */
  	__SEV();
    __WFE();
	  __WFE();
  }
  
  /* Reset SLEEPDEEP bit of Cortex System Control Register */
  SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP);  
}

/**
  * @brief  Enters Standby mode.
  * @param  None
  * @retval None
  */
void PWR_EnterStandbyMode(void)
{
  /* Clear Wake-up flag */
  PWR->CR |= PWR_CR_CWUF;
  /* Select Standby mode */
  PWR->CR |= PWR_CR_PDDS;
  /* Set SLEEPDEEP bit of Cortex System Control Register */
  SCB->SCR |= SCB_SCR_SLEEPDEEP;
/* This option is used to ensure that store operations are completed */
#if defined ( __CC_ARM   )
  __force_stores();
#endif
  /* Request Wait For Interrupt */
  __WFI();
}

/**
  * @brief  Checks whether the specified PWR flag is set or not.
  * @param  PWR_FLAG: specifies the flag to check.
  *   This parameter can be one of the following values:
  *     @arg PWR_FLAG_WU: Wake Up flag
  *     @arg PWR_FLAG_SB: StandBy flag
  *     @arg PWR_FLAG_LDORDY: Internal CPU LDO ready flag
  * @retval The new state of PWR_FLAG (SET or RESET).
  */
FlagStatus PWR_GetFlagStatus(uint32_t PWR_FLAG)
{
  FlagStatus bitstatus = RESET;
  /* Check the parameters */
  assert_param(IS_PWR_GET_FLAG(PWR_FLAG));
  
  if ((PWR->CSR & PWR_FLAG) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  /* Return the flag status */
  return bitstatus;
}

/**
  * @brief  Clears the PWR's pending flags.
  * @param  PWR_FLAG: specifies the flag to clear.
  *   This parameter can be one of the following values:
  *     @arg PWR_FLAG_WU: Wake Up flag
  *     @arg PWR_FLAG_SB: StandBy flag
  * @retval None
  */
void PWR_ClearFlag(uint32_t PWR_FLAG)
{
  /* Check the parameters */
  assert_param(IS_PWR_CLEAR_FLAG(PWR_FLAG));
         
  PWR->CR |=  PWR_FLAG << 2;
}

/**
  * @brief  Enables or disables the WakeUp Pin functionality.
  * @param  NewState: new state of the WakeUp Pin functionality.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void PWR_WakeUpPinCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
	if(NewState == ENABLE)
	{
		PWR->CSR |= PWR_EWUP1_MSK;
	}
	else
	{
		PWR->CSR &= (uint32_t)~PWR_EWUP1_MSK;
	}
}

/**
  * @brief  Config wakeup pin polarity.
  * @param  WakePolarity: specifies wakeup pin polarity.
  *   This parameter can be one of the following values:
  *     @arg RISING or FALLING
  * @retval None.
  */ 
void PWR_WakeUpPinPolarity(WakePolarity eWakePolarity)
{
  /* Check the parameters */
  if(eWakePolarity == RISING)
  {
  		PWR->WUP_POL &=  ~PWR_WUPOL_MSK;
  }
  else
  {
  		PWR->WUP_POL |=  PWR_WUPOL_MSK;  		
  }

}

/**
  * @brief  Enables or disables PDR(power down detect) when enter standby mode .
  * By setting this register, the power-off detection PDR will be turned off in Standby mode to further reduce standby power consumption (about 50nA)
  * @param  NewState: This parameter can be: ENABLE or DISABLE
  				ENABLE: power off PDR when enter standby mode.
  				DISABLE: power on PDR when enter standby mode .
  * @retval None
  */

void PWR_PowerDownDetect_Cmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

    // unlock PDR_PD Config
  PWR->PORPDR_CFG = 0x0000;
  PWR->PORPDR_CFG = 0x5500;
  PWR->PORPDR_CFG = 0xAA00;
  PWR->PORPDR_CFG = 0x5A00;
  PWR->PORPDR_CFG = 0xA500;
  PWR->PORPDR_CFG = 0xC800;
  PWR->PORPDR_CFG = 0x8C00;
  PWR->PORPDR_CFG = 0x6900;
  
    
  if(NewState == ENABLE)
 	{
      // power off  PDR
    PWR->PORPDR_CFG = 0x9601; 	
	}
	else
	{
    // power on PDR
    PWR->PORPDR_CFG = 0x9600; 
	}	  
}

/**
  * @brief  Set PMU LDO Refernce  voltage to adc.
  * @param  Vref_Set: internal Refernce out voltage  ,
  *       This parameter can be: ADC_Vref_0d8 or ADC_Vref_LDO
  				ADC_Vref_0d8: 0.8V Vref to adc.
  				ADC_Vref_LDO: LDO out Voltage to adc .(1.2V)
  * @retval None
  */

void PWR_SetLDO_RefVolToADC(uint16_t Vref_Set)
{
  uint16_t temp = 0;
     /* Check the parameters */
  assert_param(IS_PWR_VTEST_SET(Vref_Set));

/* select the LDO Voltage reference register */
  temp = PWR->VREF_SEL;

 /* Clear PDDS and LPDS bits */
  temp &= VTEST_SET_MASK;

  /* set the VREF*/
   temp |= Vref_Set;

  /* set the Register*/
   PWR->VREF_SEL |= (uint32_t)temp;

}

/**
  * @brief  Enables or disables StandbyAutoWakeupTimer .
  * @param  Timercount: timer count
  * @retval None
  */
void PWR_StandbyAutoWakeupTimer_Cmd(FunctionalState NewState)
{
   /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if(NewState == ENABLE)
  {
    PWR->CSR |= PWR_CSR_SAWU;
  }
  else
  {
    PWR->CSR &= ~PWR_CSR_SAWU;
  }
  
}


/**
  * @brief  Set stanby wakeup timer(STBAWU)
  * @param  Timercount: timer count
  * @retval None
  */
void PWR_SetStandbyWakeupTimer(uint32_t Timercount)
{
   PWR->SAWUT = Timercount;
}

/**
  * @brief  Set data to backup 0 register in standy mode
  * @param  BackupData0: data
  *  16bit side
  *  half word/word to wrrite/read
  *  data will be saved in Standby
  * @retval None
  */
void PWR_StandbyMode_BKP0(uint32_t BackupData0)
{
    PWR->SBKP0 = BackupData0;
}

/**
  * @brief  Set data to backup 1 register in standy mode
  * @param  BackupData1: data
  *  16bit side
  *  half word/word to wrrite/read
  *  data will be saved in Standby
  * @retval None
  */
void PWR_StandbyMode_BKP1(uint32_t BackupData1)
{
    PWR->SBKP0 = BackupData1;
}
