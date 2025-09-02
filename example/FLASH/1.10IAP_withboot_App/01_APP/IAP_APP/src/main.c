/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "hk32f0301mxxc.h"

#define USER_BOOT_ADDRESS       (uint32_t)(0x08000000)
#define APPLICATION_ADDRESS     (uint32_t)0x08000800
static void ledGpioInit(void);
static void ledGpioToggle(void);
static void softWareDelay(void);

typedef  void (*pFunction)(void);
static void JumpToBootapplication(void);
void time_cfg(void);

#ifdef MEM_REMAP 

#if (defined ( __CC_ARM ))
__IO uint32_t VectorTable[48] __attribute__((at(0x20000000)));
#elif (defined (__ICCARM__))
#pragma location = 0x20000000
__no_init __IO uint32_t VectorTable[48];
#elif defined   (  __GNUC__  )
__IO uint32_t VectorTable[48] __attribute__((section(".RAMVectorTable")));
#endif

#endif


uint16_t blinkcnt = 0;
int main(void)
  /* Infinite loop */
{    
#ifdef MEM_REMAP 
    uint32_t i;
      for(i=0; i<48; i++)
    {
        VectorTable[i] = *(__IO uint32_t*)(APPLICATION_ADDRESS + (i<<2));
    }
    RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_SYSCFG,ENABLE);
	 SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);
#endif
	/* LED initialization */
    ledGpioInit();	
    time_cfg();
    GPIO_SetBits(GPIOC,GPIO_Pin_7);

  while (1)
  {   
    if(blinkcnt >= 6)
    {
        blinkcnt = 0; 
        TIM_DeInit(TIM1);
        JumpToBootapplication();   
    }
  }
}




static void ledGpioInit(void)
{
	GPIO_InitTypeDef m_gpio;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	
	m_gpio.GPIO_Mode = GPIO_Mode_OUT;
	m_gpio.GPIO_OType = GPIO_OType_PP;
	m_gpio.GPIO_Pin = GPIO_Pin_7;
	m_gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	m_gpio.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOC, &m_gpio);
}

static void softWareDelay(void)
{
	uint16_t i;
	uint16_t j;
	
	for(i=0;i<500;i++)
	{
		for(j=0;j<100;j++)
		{
			__NOP();
		}
	}
}


void time_cfg(void)
{    
    TIM_TimeBaseInitTypeDef TimStruct;
    NVIC_InitTypeDef TimerNvic;
    RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_TIM1,ENABLE);
    
    TimStruct.TIM_ClockDivision = TIM_CKD_DIV1; 
    TimStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TimStruct.TIM_Period = 1000-1;
    TimStruct.TIM_Prescaler = 48-1;
    TimStruct.TIM_RepetitionCounter = 0x0000;
    TIM_TimeBaseInit(TIM1,&TimStruct);
    
    TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
    
    TimerNvic.NVIC_IRQChannel = TIM1_UP_TRG_COM_IRQn;
    TimerNvic.NVIC_IRQChannelCmd = ENABLE;
    TimerNvic.NVIC_IRQChannelPriority = 0;
    NVIC_Init(&TimerNvic);
    
    TIM_Cmd(TIM1,ENABLE);
}

static void JumpToBootapplication(void)
{
	pFunction Jump_To_BOOTApplication;
	uint32_t UserAppAddr;
	
	/* Calculate the address of user application */
    FLASH->INT_VEC_OFFSET = 0x0 ; /* Vector Table Relocation in Internal FLASH. */
	UserAppAddr = *(__IO uint32_t*)(USER_BOOT_ADDRESS + 4);
	Jump_To_BOOTApplication = (pFunction) UserAppAddr;
	
	/* Initialize user application's Stack Pointer, Disable any interrupts during SP setting */
	__set_PRIMASK(1); // Disable interrupts
  
	__set_MSP(*(__IO uint32_t*) USER_BOOT_ADDRESS);
	__set_PRIMASK(0); // Enable interrupts

	/* Jump to application */
	Jump_To_BOOTApplication();
}
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char* file , uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */	
       /* Infinite loop */
	
	while (1)
  {		
  }
}
#endif /* USE_FULL_ASSERT */


