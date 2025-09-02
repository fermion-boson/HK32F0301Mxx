/**
  ******************************************************************************
  * @file           : iic.c
  * @brief          : iic example
  ******************************************************************************
  * @attention
  *
  */

#include "iic.h"

/**
  * @brief  I2C config.
  * @retval None
  */
void I2C_Config(void)
{
	I2C_InitTypeDef		I2C_InitStructure;
	GPIO_InitTypeDef	GPIO_InitStructure;

	/* Enable clock */
	I2C_CLK_CMD(I2C_CLK,ENABLE);
	I2C_IOCLK_CMD(I2C_IOCLK,ENABLE);
	
	/* Initializes the GPIOx peripheral */
	GPIO_InitStructure.GPIO_Pin = I2C_CK_PIN|I2C_DA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	       
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(I2C_IO_PORT, &GPIO_InitStructure);

	/* selects the pin to used as Alternate function */
	GPIO_PinAFConfig(I2C_IO_PORT,I2C_AF_CK_PIN,I2C_AF_SELECT);
	GPIO_PinAFConfig(I2C_IO_PORT,I2C_AF_DA_PIN,I2C_AF_SELECT);
		
    /* Don't Exchange SDA/SCL pin */
    I2C_SwapCmd(I2C,DISABLE);
    
	/* Initializes the I2Cx peripheral */
	I2C_InitStructure.I2C_Timing = 0x00200002;
	I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Disable;
	I2C_InitStructure.I2C_DigitalFilter = 0;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;	
	I2C_InitStructure.I2C_OwnAddress1 = 0x32; 
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2C, &I2C_InitStructure);
	
	/*Wake-up from stop requires clock stretching to be enabled */
	I2C_StretchClockCmd(I2C,ENABLE);
	
	/* Enables I2C */
	I2C_Cmd(I2C, ENABLE);  	
}




