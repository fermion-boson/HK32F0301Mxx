******************** (C) COPYRIGHT 2022 HK Microchip*******************
* @file      Example\TIM\TIM_deadTime_break_syscfg
* @author    HK Microchip AE Team
* @version   V1.0.0
* @date      2022.09.24
* @brief     cortex_M0 lockup使能TIM1刹车
***********************************************************************


****************************** 例程描述********************************
功能描述：
	 本例程描述了TIM1互补输出死区，使用cortex_M0 lockup使能TIM1刹车。
           配置刹车功能和刹车引脚PB5(仿真时应屏蔽掉PB5的配置，因为PB5为SWD功能引脚).
           两次进入hardfault,cortex_M0 lockup=1(实际上发生，但读不到)，
	 配置SYSCFG_CFGR1.LOCKUP=1，使TIM1互补输出刹车。使程序中使用示波器来查看实验结果

时钟配置：
	本例程采用HSI32MHz作为系统时钟，关于系统时钟如何配置，请参考HK32F0301MxxC用户
	手册或者RCC库函数及RCC相关例程。
	

适用芯片：
	HK32F0301MxxC
涉及外设：
	TIM/RCC

注意事项：
	1. 推荐使用MDK5.00或IAR8.22.1及其以上版本编译本工程；
	2. 使用之前检测电源VCC、GND等是否正常。

如何使用：
	1. 配置SYSCFG->CFGR1=0<<31;编译，下载
	2. 用示波器连接PC6（TIM1_CH1）,PA1(TIM1_CH1N)，查看有互补输出的方波（cortex_M0 lockup没有连接到刹车）
	3. 配置SYSCFG->CFGR1=1<<31;编译，下载
	4. 用示波器连接PC6（TIM1_CH1）,PA1(TIM1_CH1N)，查看无互补输出的方波（cortex_M0 lockup触发了刹车）
	5. 如何触发cortex_M0 lockup=1。需要两次进入hardfault。通过仿真可以看到（需屏蔽掉PB5的配置）
                6. main.c此处p1 = p_Hardfault();第一次进入hard fault。 it.c的此处p2=hardfault();第二次进入hard fault。然后cortex_M0 lockup=1
	7.cortex_M0 lockup=1，如果cortex_M0 lockup连接到刹车上，就刹车。如果没连上就不刹车（此时要恢复PB5的配置）
	
	
	
***********************************************************************


****************************** 版权申明********************************
本例程所有版权归深圳市航顺芯片技术研发有限公司2015 - 2021
***********************************************************************
