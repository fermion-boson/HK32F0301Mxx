/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************

  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#include "hk32f0301mxxc.h"

extern uint32_t __IO TimeCount;

#ifdef __cplusplus
extern "C" {
#endif

void TimingDelay_Decrement(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

