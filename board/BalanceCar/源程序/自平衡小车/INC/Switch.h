#ifndef _SWITCH_H_
#define _SWITCH_H_

#include "stm32f10x.h"
#include "sys.h"

#define Switch1   PBin(12)
#define Switch2   PBin(13)
#define Switch3   PBin(14)
#define Switch4   PBin(15)

void Switch_Init(void);

#endif

