#include "Key.h"

uint8_t Key1_Count,Key2_Count,Key3_Count,Key4_Count;

void Key_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

    //LED引脚配置，PC08~PC15
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void KeyScan(void)
{
  if(Key1==0)  //按键1被按下
    {
//	   Delay_ms(10);
//	   if(Key1==0)
//	     {
		   Key1_Count=1;  //按键1标志位置1
		   kp=kp+5;		  //直立环kp系数加5
		   while(!Key1);  //等待按键释放	
//		 }
	}

  if(Key2==0)	//按键2被按下
    {
//	   Delay_ms(10);
//	   if(Key2==0)
//	     {
		   Key2_Count=1; //按键2标志位置1
		   kp=kp-5;;	 //直立环kp系数减5
		   while(!Key2); //等待按键释放	
//		 }
	}
  
  if(Key3==0)	//按键3被按下
    {
//	   Delay_ms(10);
//	   if(Key3==0)
//	     {
		   Key3_Count=1;  //按键3标志位置1
		   if(Switch2==0) //拨码开关2打开
		        kd++;	  //直立环kd系数加1
		   else			  //拨码开关2关闭
		        Midle=Midle+0.05; //小车机械零点加0.05
		   while(!Key3);	//等待按键释放
//		 }
	}

  if(Key4==0)	//按键4被按下
    {
//	   Delay_ms(10);
//	   if(Key4==0)
//	     {
		   Key4_Count=1;  //按键4标志位置1
		   if(Switch2==0) //拨码开关2打开
		      kd--;		  //直立环kd系数减1
		   else			  //拨码开关2关闭
		      Midle=Midle-0.05; //小车机械零点减0.05		   
		   while(!Key4);  //等待按键释放	
//		 }
	}
}

//KeyScan();
//	  OLED_ShowNum(0,2,Key1_Count,3,16);//显示ASCII字符的码值
//	  OLED_ShowNum(0,4,Key2_Count,3,16);//显示ASCII字符的码值
//	  OLED_ShowNum(0,6,Key3_Count,3,16);//显示ASCII字符的码值
//	  OLED_ShowNum(103,6,Key4_Count,3,16);//显示ASCII字符的码值

