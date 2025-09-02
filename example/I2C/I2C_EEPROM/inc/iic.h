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

/* EEPROM type */
#define HK24C01  			(0x01)
#define HK24C02  			(0x02)
#define HK24C04  			(0x03)
#define HK24C08  			(0x04)
#define HK24C16  			(0x05)
#define HK24C32  			(0x06)
#define HK24C64  			(0x07)
#define HK24C128 			(0x08)
#define HK24C256 			(0x09)
#define HK24C512 			(0x0a)
#define HK24C1024			(0x0b)
#define EE_TYPE 			HK24C02
	
/* I2C PIN */	
#define I2C_CK_PIN			GPIO_Pin_6
#define I2C_DA_PIN			GPIO_Pin_5
#define	I2C_IO_PORT			GPIOC
#define I2C_AF_CK_PIN		GPIO_PinSource6
#define I2C_AF_DA_PIN		GPIO_PinSource5

/* I2C CLOCK */
#define I2C_CLK_CMD			RCC_APBPeriph1ClockCmd
#define I2C_CLK				RCC_APBPeriph1_I2C

/* AF selection */
#define I2C_AF_SELECT 		GPIO_AF_0

/* I2C IO CLOCK */
#define I2C_IOCLK_CMD		RCC_AHBPeriphClockCmd
#define I2C_IOCLK			RCC_AHBPeriph_GPIOC

/* Exported macro ------------------------------------------------------------*/
void I2C_Config(void);
void I2C_Read(uint16_t SalveAddr,uint8_t startaddr,uint8_t *buffer,uint8_t Length);
void I2C_Write(uint16_t SalveAddr,uint8_t startaddr,uint8_t *buffer, uint8_t Length);

#ifdef __cplusplus
}
#endif

#endif


