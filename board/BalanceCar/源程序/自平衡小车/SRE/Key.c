#include "Key.h"

uint8_t Key1_Count,Key2_Count,Key3_Count,Key4_Count;

void Key_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

    //LED�������ã�PC08~PC15
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void KeyScan(void)
{
  if(Key1==0)  //����1������
    {
//	   Delay_ms(10);
//	   if(Key1==0)
//	     {
		   Key1_Count=1;  //����1��־λ��1
		   kp=kp+5;		  //ֱ����kpϵ����5
		   while(!Key1);  //�ȴ������ͷ�	
//		 }
	}

  if(Key2==0)	//����2������
    {
//	   Delay_ms(10);
//	   if(Key2==0)
//	     {
		   Key2_Count=1; //����2��־λ��1
		   kp=kp-5;;	 //ֱ����kpϵ����5
		   while(!Key2); //�ȴ������ͷ�	
//		 }
	}
  
  if(Key3==0)	//����3������
    {
//	   Delay_ms(10);
//	   if(Key3==0)
//	     {
		   Key3_Count=1;  //����3��־λ��1
		   if(Switch2==0) //���뿪��2��
		        kd++;	  //ֱ����kdϵ����1
		   else			  //���뿪��2�ر�
		        Midle=Midle+0.05; //С����е����0.05
		   while(!Key3);	//�ȴ������ͷ�
//		 }
	}

  if(Key4==0)	//����4������
    {
//	   Delay_ms(10);
//	   if(Key4==0)
//	     {
		   Key4_Count=1;  //����4��־λ��1
		   if(Switch2==0) //���뿪��2��
		      kd--;		  //ֱ����kdϵ����1
		   else			  //���뿪��2�ر�
		      Midle=Midle-0.05; //С����е����0.05		   
		   while(!Key4);  //�ȴ������ͷ�	
//		 }
	}
}

//KeyScan();
//	  OLED_ShowNum(0,2,Key1_Count,3,16);//��ʾASCII�ַ�����ֵ
//	  OLED_ShowNum(0,4,Key2_Count,3,16);//��ʾASCII�ַ�����ֵ
//	  OLED_ShowNum(0,6,Key3_Count,3,16);//��ʾASCII�ַ�����ֵ
//	  OLED_ShowNum(103,6,Key4_Count,3,16);//��ʾASCII�ַ�����ֵ

