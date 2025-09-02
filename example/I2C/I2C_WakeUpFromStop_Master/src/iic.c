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
  * @brief  I2C write.
  * @retval None
  */
void IIC_Write(uint16_t SalveAddr,uint8_t startaddr,uint8_t *buffer, uint8_t Length)
{
	uint8_t i;
	while(I2C_GetFlagStatus(I2C,I2C_FLAG_BUSY));

#if (EE_TYPE > HK24C16)	
	I2C_TransferHandling(I2C,SalveAddr,2,I2C_Reload_Mode,I2C_Generate_Start_Write);
	/*检查TXDR寄存器是否为空*/
	while(I2C_GetFlagStatus(I2C,I2C_FLAG_TXIS)==RESET);
	
	I2C_SendData(I2C,startaddr);
	/*该位在Reload=1时NBYTES个数数据发送完成后被置1，向NBYTES写入非0数值时被清0*/
	while(I2C_GetFlagStatus(I2C,I2C_FLAG_TXIS)==RESET);
	I2C_SendData(I2C,startaddr);
	while(I2C_GetFlagStatus(I2C,I2C_FLAG_TCR)==RESET);

#else 
	I2C_TransferHandling(I2C,SalveAddr,1,I2C_Reload_Mode,I2C_Generate_Start_Write);
	/*检查TXDR寄存器是否为空*/
	while(I2C_GetFlagStatus(I2C,I2C_FLAG_TXIS)==RESET);
	
	I2C_SendData(I2C,startaddr);
	while(I2C_GetFlagStatus(I2C,I2C_FLAG_TCR)==RESET);
#endif 
	
	I2C_TransferHandling(I2C,SalveAddr,Length,I2C_AutoEnd_Mode,I2C_No_StartStop);
	
	for(i=0;i<Length;i++)
	{
		/*等待发送寄存器为空*/
		while(I2C_GetFlagStatus(I2C,I2C_FLAG_TXIS)==RESET);
		
		I2C_SendData(I2C,buffer[i]);	
	}
	/*作为主机检测由外设产生的停止信号*/
	while(I2C_GetFlagStatus(I2C,I2C_FLAG_STOPF)==RESET);
	
	I2C_ClearFlag(I2C,I2C_FLAG_STOPF);
}

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
	I2C_InitStructure.I2C_OwnAddress1 = 0; 
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;	 
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2C, &I2C_InitStructure);
	
	/* Enables I2C */
	I2C_Cmd(I2C, ENABLE);
}


