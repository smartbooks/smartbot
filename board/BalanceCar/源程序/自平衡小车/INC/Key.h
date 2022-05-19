#ifndef _KEY_H_
#define _KEY_H_

#include "stm32f10x.h"
#include "sys.h"
#include "systic.h"
#include "mpu6050.h"
#include "OLED.h"
#include "LED.h"
#include "Switch.h"

#define Key1   PEin(7)
#define Key2   PEin(9)
#define Key3   PEin(11)
#define Key4   PEin(13)

extern uint8_t Key1_Count,Key2_Count,Key3_Count,Key4_Count;

void Key_Init(void);
void KeyScan(void);

#endif

