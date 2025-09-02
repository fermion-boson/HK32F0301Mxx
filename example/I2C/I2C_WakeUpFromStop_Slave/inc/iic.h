/**
  ******************************************************************************
  * @file           : iic.h
  * @brief          : iic example
  ******************************************************************************
  * @attention
  *
  */

#ifndef __IIC_H
#define __IIC_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "hk32f0301mxxc.h"

/* I2C PIN */
#define I2C_CK_PIN				GPIO_Pin_6
#define I2C_DA_PIN				GPIO_Pin_5
#define	I2C_IO_PORT				GPIOC
#define I2C_AF_CK_PIN			GPIO_PinSource6
#define I2C_AF_DA_PIN			GPIO_PinSource5

/* I2C CLOCK */
#define I2C_CLK_CMD				RCC_APBPeriph1ClockCmd
#define I2C_CLK					RCC_APBPeriph1_I2C

/* AF selection */
#define I2C_AF_SELECT 			GPIO_AF_0

/* I2C IO CLOCK */
#define I2C_IOCLK_CMD			RCC_AHBPeriphClockCmd
#define I2C_IOCLK				RCC_AHBPeriph_GPIOC

/* Exported macro ------------------------------------------------------------*/
void I2C_Config(void);

#ifdef __cplusplus
}
#endif

#endif


