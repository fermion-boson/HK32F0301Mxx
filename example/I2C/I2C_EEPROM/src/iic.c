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
  * @brief  Configure I2C.
  * @retval None
  */
void I2C_Config(void)
{
	I2C_InitTypeDef		I2C_InitStructure;
	GPIO_InitTypeDef	GPIO_InitStructure;
	
	/* Enables clock */
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
	I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
	I2C_InitStructure.I2C_DigitalFilter = 4;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1 = 0; 
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2C, &I2C_InitStructure);
	
	/* Enables I2C */
	I2C_Cmd(I2C, ENABLE);  	
}

/**
  * @brief  I2C read data.
  * @retval None
  */
void I2C_Read(uint16_t SalveAddr,uint8_t startaddr,uint8_t *buffer,uint8_t Length)
{
	uint8_t i;
	
	/*��������Ƿ�æ*/
	while(I2C_GetFlagStatus(I2C,I2C_FLAG_BUSY));
	
#if (EE_TYPE > HK24C16)
	I2C_TransferHandling(I2C,SalveAddr,2,I2C_SoftEnd_Mode,I2C_Generate_Start_Write);
	/*���TXDR�Ĵ����Ƿ�Ϊ��*/
	while(I2C_GetFlagStatus(I2C,I2C_FLAG_TXIS)==RESET);
	/*�����߷��ʹ�������ַ*/
	I2C_SendData(I2C, startaddr);
	/*��鷢���Ƿ����*/
	while(I2C_GetFlagStatus(I2C,I2C_FLAG_TXIS)==RESET);
	 
	I2C_SendData(I2C,startaddr);
	while(I2C_GetFlagStatus(I2C,I2C_FLAG_TC)==RESET);
	
	
#else
	I2C_TransferHandling(I2C,SalveAddr,1,I2C_SoftEnd_Mode,I2C_Generate_Start_Write);
	/*���TXDR�Ĵ����Ƿ�Ϊ��*/
	while(I2C_GetFlagStatus(I2C,I2C_FLAG_TXIS)==RESET);
	/*�����߷��ʹ�������ַ*/
	I2C_SendData(I2C,startaddr);
	while(I2C_GetFlagStatus(I2C,I2C_FLAG_TC)==RESET);
#endif
	/*����һ��������ʼ�ź�*/
	I2C_TransferHandling(I2C,SalveAddr,Length,I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
	
	for(i=0;i<Length;i++)
	{
		/*�ȴ��������*/
		while(I2C_GetFlagStatus(I2C,I2C_FLAG_RXNE)==RESET);
		
		buffer[i]=I2C_ReceiveData(I2C);	
	}
	/*��Ϊ������������������ֹͣ�ź�*/	
	while(I2C_GetFlagStatus(I2C,I2C_FLAG_STOPF)==RESET);
	I2C_ClearFlag(I2C,I2C_FLAG_STOPF);
}

/**
  * @brief  I2C write data.
  * @retval None
  */
void I2C_Write(uint16_t SalveAddr,uint8_t startaddr,uint8_t *buffer, uint8_t Length)
{
	uint8_t i;
	while(I2C_GetFlagStatus(I2C,I2C_FLAG_BUSY));

#if (EE_TYPE > HK24C16)	
	I2C_TransferHandling(I2C,SalveAddr,2,I2C_Reload_Mode,I2C_Generate_Start_Write);
	/*���TXDR�Ĵ����Ƿ�Ϊ��*/
	while(I2C_GetFlagStatus(I2C,I2C_FLAG_TXIS)==RESET);
	
	I2C_SendData(I2C,startaddr);
	/*��λ��Reload=1ʱNBYTES�������ݷ�����ɺ���1����NBYTESд���0��ֵʱ����0*/
	while(I2C_GetFlagStatus(I2C,I2C_FLAG_TXIS)==RESET);
	I2C_SendData(I2C,startaddr);
	while(I2C_GetFlagStatus(I2C,I2C_FLAG_TCR)==RESET);

#else 
	I2C_TransferHandling(I2C,SalveAddr,1,I2C_Reload_Mode,I2C_Generate_Start_Write);
	/*���TXDR�Ĵ����Ƿ�Ϊ��*/
	while(I2C_GetFlagStatus(I2C,I2C_FLAG_TXIS)==RESET);
	
	I2C_SendData(I2C,startaddr);
	while(I2C_GetFlagStatus(I2C,I2C_FLAG_TCR)==RESET);
#endif 
	
	I2C_TransferHandling(I2C,SalveAddr,Length,I2C_AutoEnd_Mode,I2C_No_StartStop);
	
	for(i=0;i<Length;i++)
	{
		/*�ȴ����ͼĴ���Ϊ��*/
		while(I2C_GetFlagStatus(I2C,I2C_FLAG_TXIS)==RESET);
		
		I2C_SendData(I2C,buffer[i]);	
	}
	/*��Ϊ������������������ֹͣ�ź�*/
	while(I2C_GetFlagStatus(I2C,I2C_FLAG_STOPF)==RESET);
	
	I2C_ClearFlag(I2C,I2C_FLAG_STOPF);
}




