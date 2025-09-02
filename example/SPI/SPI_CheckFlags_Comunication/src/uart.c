
#include "uart.h"
#include "stdio.h"
#include "stdarg.h"
#include "main.h"


/**
  * @brief  Configures COM port.
  * @param  UART_InitStruct: pointer to a UART_InitTypeDef structure that
  *         contains the configuration information for the specified UART peripheral.
  * @retval None
  */
void UART_COMInit(UART_InitTypeDef* UART_InitStruct)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIO clock */
  RCC_AHBPeriphClockCmd(UART_COM_TX_GPIO_CLK | UART_COM_RX_GPIO_CLK, ENABLE);

  /* Enable UART clock */
  RCC_APBPeriph2ClockCmd(UART_COM_CLK, ENABLE); 

  /* Connect PXx to UARTx_Tx */
  GPIO_PinAFConfig(UART_COM_TX_GPIO_PORT, UART_COM_TX_SOURCE, UART_COM_TX_AF);

  /* Connect PXx to UARTx_Rx */
  GPIO_PinAFConfig(UART_COM_RX_GPIO_PORT, UART_COM_RX_SOURCE, UART_COM_RX_AF);
  
  /* Configure UART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = UART_COM_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(UART_COM_TX_GPIO_PORT, &GPIO_InitStructure);
    
  /* Configure UART Rx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = UART_COM_RX_PIN;
  GPIO_Init(UART_COM_RX_GPIO_PORT, &GPIO_InitStructure);

  /* UART configuration */
  UART_Init(UART_COM, UART_InitStruct);
    
  /* Enable UART */
  UART_Cmd(UART_COM, ENABLE);
}

/*
 * ��������fputc
 * ����  ���ض���c�⺯��printf��UART1
 * ����  ����
 * ���  ����
 * ����  ����printf����
 */
PUTCHAR_PROTOTYPE
{
    /* ��Printf ���ݷ������� */
    UART_SendData(UART_COM, (unsigned char) ch);
    while(UART_GetFlagStatus(UART_COM, UART_FLAG_TC) != SET);
    return (ch);
}

/*
 * ��������itoa
 * ����  ������������ת�����ַ���
 * ����  ��-radix=10 ��ʾʮ���ƣ��������Ϊ0
           -value Ҫת����������
 *         -string ת������ַ���
 * ���  ����
 * ����  ����UART1_printf()����
 */
static char *itoa(int value, char *string, int radix)
{
    int  i,d;
    int  flag = 0;
    char *ptr = string;
    
    /* �˴���֧��ʮ����ת�� */
    if(radix != 10)
    {
        *ptr = 0;
        return string;
    }
    
    if(!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }
    
    /* �����һ�����ţ��ͼ��ϡ�-������ */
    if(value < 0)
    {
        *ptr++ = '-';
        value *= -1;
    }
    for(i=10000; i>0; i/=10)
    {
        d = value / i;
        if(d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }
    
    /* ����ֹ�ַ�. */
    *ptr = 0;
    return string;
}

/*
 * ��������UART1_printf
 * ����  ����ʽ�������������C ���е�printf��������û���õ�C��
 * ����  ��-UARTx ����ͨ��������ֻ�õ��˴���1����UART1
 *         -Data Ҫ���͵����ڵ����ݵ�ָ��
 * ���  ����
 * ����  ���ⲿ����
 */
void UART_printf(UART_TypeDef* UARTx, const char *Data,...)
{
    const char *s;
    int d;
    char buf[16];
    va_list ap;
    va_start(ap,Data);
    while(*Data != 0)           //�ж��Ƿ񵽴��ַ���������
    {
        if(*Data == 0x5c)       //'\'
        {
            switch(*++Data)
            {
                case 'r':       //�س���
                    UART_SendData(UARTx, 0x0d);
                    Data++;
                    break;
                
                case 'n':       //���з�
                    UART_SendData(UARTx, 0x0a);
                    Data++;
                    break;
                default:
                    Data++;
                    break;
            }
        }
        else if(*Data == '%')
        {
            switch(*++Data)
            {
                case 's':       //�ַ���
                    s = va_arg(ap, const char *);
                    for(; *s; s++)
                    {
                        UART_SendData(UARTx, *s);
                        while(UART_GetFlagStatus(UARTx, UART_FLAG_TC) == RESET); 
                    }
                    Data++;
                    break;
                case 'd':       //ʮ����
                    d = va_arg(ap, int);
                    itoa(d, buf, 10);
                    for(s=buf; *s; s++)
                    {
                        UART_SendData(UARTx, *s);
                        while(UART_GetFlagStatus(UARTx, UART_FLAG_TC) == RESET);
                    }
                    Data++;
                    break;
                default:
                    Data++;
                    break;
            }
        }
        else
        {
            UART_SendData(UARTx, *Data++);
        }
        while(UART_GetFlagStatus(UARTx, UART_FLAG_TC) == RESET);
    }
}

/**
  * @brief  Gets numeric values from the hyperterminal.
  * @param  None
  * @retval None
  */
uint8_t UART_Scanf(uint32_t value, uint32_t count)
{
  uint32_t index = 0;
  uint32_t tmp[5] = {0, 0};

  while (index < count)
  {
    /* Loop until RXNE = 1 */
    while (UART_GetFlagStatus(UART_COM, UART_FLAG_RXNE) == RESET)
    {}
    tmp[index++] = (UART_ReceiveData(UART_COM));
    if ((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39))
    {
      printf("\n\rPlease enter valid number between 0 and 9");
      index--;
    }
  }
  
  if(count == 1)
  {
    index = tmp[0] - 0x30;
  }
  else if(count == 2)
  {
    /* Calculate the Corresponding value */
    index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10);
  }
  else if(count == 3)
  {
    /* Calculate the Corresponding value */
    index = (tmp[2] - 0x30) + ((tmp[1] - 0x30) * 10) + ((tmp[0] - 0x30) * 100);
  }
  else if(count == 4)
  {
    /* Calculate the Corresponding value */
    index = (tmp[3] - 0x30) + ((tmp[2] - 0x30) * 10) + ((tmp[1] - 0x30) * 100) + ((tmp[0] - 0x30) * 1000);
  }
  else if(count == 5)
  {
    /* Calculate the Corresponding value */
    index = (tmp[4] - 0x30) + ((tmp[3] - 0x30) * 10) + ((tmp[2] - 0x30) * 100) + ((tmp[1] - 0x30) * 1000) + ((tmp[0] - 0x30) * 10000);
  }
  else
  {
  
  }
  
  /* Checks */
  if (index > value)
  {
    printf("\n\rPlease enter valid number between 0 and %d", value);
    return 0xFF;
  }
  return index;
}

