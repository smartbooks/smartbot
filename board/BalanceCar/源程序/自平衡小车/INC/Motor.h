#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "stm32f10x.h"
#include "sys.h"
#include "systic.h"
#include "MPU6050.h"

#define AIN1  PCout(6)
#define AIN2  PDout(15)
#define BIN1  PCout(7)
#define BIN2  PCout(8)

#define PWMA  TIM2->CCR1  //PA15
#define PWMB  TIM2->CCR2  //PB3

extern int Moto1,Moto2;

void PWM_Init(uint16_t arr,uint16_t psc);
void Set_Pwm(int moto1,int moto2);
void Xianfu_Pwm(void);
void Stop_Move(void);

#endif


