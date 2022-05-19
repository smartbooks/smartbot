#include "TIM.h"

void TIM_Config(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure; 
	//TIM2�ж����ȼ�����     
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);						/* ����ʱ�� */
    TIM_DeInit(TIM3);
    TIM_TimeBaseStructure.TIM_Period= 99;		 								/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    TIM_TimeBaseStructure.TIM_Prescaler= 7199;				    /* ʱ��Ԥ��Ƶ�� 72M/7200=10k */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* ������Ƶ */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* ���ϼ���ģʽ */

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);	 //���ؼ���ֵ
	TIM_ARRPreloadConfig(TIM3, ENABLE);          	 //���ؼ���ֵ

    TIM_ClearFlag(TIM3, TIM_FLAG_Update);							    		/* �������жϱ�־ */
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);

    TIM_Cmd(TIM3, ENABLE);				
}


