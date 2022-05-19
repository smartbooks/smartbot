#include "systic.h"

uint32_t TimingDelay;

/*
 * 函数名：SysTick_Init
 * 描述  ：启动系统滴答定时器 SysTick
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用 
 */
void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
	NVIC_SetPriority(SysTick_IRQn, 0x06); 
	if (SysTick_Config(SystemCoreClock / 1000000))	// ST3.5.0库版本
	{ 
		/* Capture error */ 
		while (1);
	}
		// 关闭滴答定时器  
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
		// 使能滴答定时器  
//	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;

}

void Delay_us(__IO uint32_t nTime)
{ 
	TimingDelay = nTime;
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;	  	// 使能滴答定时器  
	while(TimingDelay != 0);
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;	   	// 关闭滴答定时器 
}

void Delay_ms(__IO uint32_t nTime)
{ 
		Delay_us(nTime*1000);
}

void Delay_s(__IO uint32_t nTime)
{ 
		Delay_us(nTime*1000000);
}

