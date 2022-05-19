#include "TIM.h"

void TIM_Config(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure; 
	//TIM2中断优先级配置     
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);						/* 开启时钟 */
    TIM_DeInit(TIM3);
    TIM_TimeBaseStructure.TIM_Period= 99;		 								/* 自动重装载寄存器周期的值(计数值) */
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    TIM_TimeBaseStructure.TIM_Prescaler= 7199;				    /* 时钟预分频数 72M/7200=10k */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* 采样分频 */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* 向上计数模式 */

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);	 //重载计数值
	TIM_ARRPreloadConfig(TIM3, ENABLE);          	 //重载计数值

    TIM_ClearFlag(TIM3, TIM_FLAG_Update);							    		/* 清除溢出中断标志 */
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);

    TIM_Cmd(TIM3, ENABLE);				
}


