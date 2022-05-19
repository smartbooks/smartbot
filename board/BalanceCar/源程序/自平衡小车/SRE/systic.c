#include "systic.h"

uint32_t TimingDelay;

/*
 * ��������SysTick_Init
 * ����  ������ϵͳ�δ�ʱ�� SysTick
 * ����  ����
 * ���  ����
 * ����  ���ⲿ���� 
 */
void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms�ж�һ��
	 * SystemFrequency / 100000	 10us�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��
	 */
	NVIC_SetPriority(SysTick_IRQn, 0x06); 
	if (SysTick_Config(SystemCoreClock / 1000000))	// ST3.5.0��汾
	{ 
		/* Capture error */ 
		while (1);
	}
		// �رյδ�ʱ��  
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
		// ʹ�ܵδ�ʱ��  
//	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;

}

void Delay_us(__IO uint32_t nTime)
{ 
	TimingDelay = nTime;
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;	  	// ʹ�ܵδ�ʱ��  
	while(TimingDelay != 0);
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;	   	// �رյδ�ʱ�� 
}

void Delay_ms(__IO uint32_t nTime)
{ 
		Delay_us(nTime*1000);
}

void Delay_s(__IO uint32_t nTime)
{ 
		Delay_us(nTime*1000000);
}

