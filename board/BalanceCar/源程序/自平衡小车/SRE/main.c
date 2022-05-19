/*	Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stdio.h"
#include "systic.h"
#include "OLED.h" 
#include "MPU6050.h"
#include "LED.h" 
#include "Buzzer.h"
#include "Switch.h"
#include "Key.h"
#include "Motor.h"
#include "USART.h"
#include "TIM.h"
/*	define---------------------------------------------------------------------*/
uint32_t Main_Count = 0;
uint8_t  Key_num = 0;
uint32_t Test;
uint8_t Temp[20];

int main(void)
{
  	USART_NVIC_Configuration();					// 配置串口中断
 	USART_Config();								// 配置串口
    SysTick_Init();	    //滴答定时器初始化 
	OLED_Init();	    //OLED引脚初始化 
	OLED_Clear();	    //OLED清屏 
	OLED_First_Line();  //OLED第一行显示: 贺梓洹毕业设计
	LED_Init();		    //LED引脚初始化
	Speak_Init();	    //蜂鸣器引脚初始化
	Switch_Init();	    //拨码开关引脚初始化
	Key_Init();		    //按键引脚初始化
	MPU6050_Init();
	PWM_Init(7199,0);
	TIM_Config();
//	Speak=1;
//	Delay_s(1);
//	Speak=0;
	while(1)
	{
//	  KeyScan();

//	  if(Key1_Count==1)
//	  {
//	  	Key1_Count=0;
//		kp=kp+3;
//	  }
//	  OLED_ShowNum(0,6,Key1_Count,6,16);//显示ASCII字符的码值
//	  OLED_ShowNum(0,4,Key2_Count,6,16);//显示ASCII字符的码值
//	  OLED_ShowNum(0,6,Key3_Count,3,16);//显示ASCII字符的码值
//	  OLED_ShowNum(103,6,Key4_Count,3,16);//显示ASCII字符的码值
//	  Delay_ms(5);
//	  OLED_ShowNum(103,6,Key4_Count,3,16);//显示ASCII字符的码值 
//	 LED2=0;
//	 LED3=0;
//	 LED4=0;
//	 Speak=0;
//	 Delay_s(1);
////	 LED1=1;
//	 LED2=1;
//	 LED3=1;
//	 LED4=1;
//	 Speak=1;
//	 Delay_s(1);

//	  	   READ_MPU6050();
//		   Kalman_Filter(Data_6050.Acc_X , Data_6050.Gryo_Y);	//?üD????????èoí???????ù?è	
////	   //GyroX++;
//	   OLED_ShowNum(103,2,Data_6050.Acc_X,3,16);//显示ASCII字符的码值 
////	   //GyroY++;
//	   OLED_ShowNum(103,4,Data_6050.Gryo_Y,3,16);//显示ASCII字符的码值 
////	   //GyroZ++;
//	   OLED_ShowNum(103,6,angle,3,16);//显示ASCII字符的码值 
//	   Delay_ms(100);
	}
}
