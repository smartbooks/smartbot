#ifndef _BUZZER_H_
#define _BUZZER_H_

#include "stm32f10x.h"
#include "sys.h"

#define Speak   PDout(6)

void Speak_Init(void);

#endif

