#ifndef _LED_H_
#define _LED_H_

#include "stm32f10x.h"
#include "sys.h"

#define LED1   PEout(8)
#define LED2   PEout(10)
#define LED3   PEout(12)
#define LED4   PEout(14)

void LED_Init(void);

#endif
