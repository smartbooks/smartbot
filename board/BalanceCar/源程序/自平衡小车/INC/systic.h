#ifndef __SYSTIC_H
#define	__SYSTIC_H

#include "stm32f10x.h"

void SysTick_Init(void);
void Delay_us(__IO uint32_t nTime);
void Delay_ms(__IO uint32_t nTime);
void Delay_s(__IO uint32_t nTime) ;

#endif

