#include <reg52.h>
#include <absacc.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char
#define Phone_connection  1    //���ڴ�绰״̬��־
#define Clear_Connect     0    //�绰�ѹҶ�״̬��־
sbit Phone_Call_Key=P1^0;     //��绰����
sbit Hang_up=P1^1;     //�ҵ绰���� 

sbit LED0=P0^0;
sbit LED1=P0^1;
sbit LED2=P0^2;
sbit LED3=P0^3;
sbit LED4=P0^4;
sbit LED5=P0^5;
sbit LED6=P0^6;
sbit LED7=P0^7;

char Calls_State_Flag=Clear_Connect;   //�绰״̬  
uchar code PhoneCall[]="02228214113";
uchar code ATH[]="ATH";
void delay(uint ms)// ��ʱ�ӳ���
{      
uchar i;
while(ms--)
{
  for(i=0;i<120;i++);
}
}

/*********************************************************** 
�������ƣ�Print_Char 
�������ܣ����͵����ַ�     
          ��ڲ���:ch      ���ڲ������� 
***********************************************************/
void Print_Char(uchar ch)//���͵����ַ�
{
SBUF=ch; //���뻺����
while(TI==0); //�ȴ��������
TI=0; //�������
}

/*********************************************************** 
�������ƣ�Print_Str 
�������ܣ������ַ���     
          ��ڲ���:*str    ���ڲ������� 
***********************************************************/
Print_Str(uchar *str)//�����ַ���
{

while(*str!='\0') 
{
Print_Char(*str++);
    }

}
/*Print_Str(uchar *str,uint len)//�����ַ���,����Send_Char() len�ַ�������
{

while(len--)
{
Print_Char(*str++);
    }

}*/

 
/*********************************************************** 
�������ƣ�Ini_UART 
�������ܣ����ڳ�ʼ������ʱ����ʼ��     
          ��ڲ���:��     ���ڲ������� 
***********************************************************/ 
Ini_UART(void)//���ڳ�ʼ������ʱ����ʼ��
{
TMOD = 0x20;      //T1��ʽ2,����UART������ 
TH1 = 0xFD;       //UART����������:9600 
TL1 = 0xFD;
SCON = 0x50;  ////UART��ʽ1:8λUART;   REN=1:������� 
PCON = 0x00;

TF1 = 1;     // �жϱ�־λ
TR1 = 1;    // ������ʱ��1    
ES=1;       //�������п��ж�
ET0=1;      //�����жϹ���
EA=1;
}
 
/*********************************************************** 
�������ƣ�GSM_ATH 
�������ܣ��ҵ绰      
          ��ڲ���:��     ���ڲ������� 
***********************************************************/ 
void GSM_ATH()   
{   
     Print_Str(ATH); //���͹ҵ绰ָ��
     Print_Str("\r\n");   //���ͻس�
      
     //delay(1000); 
     //delay(1000); 
}  
	
/*********************************************************** 
�������ƣ�phone 
�������ܣ�����Ŀ��绰      
          ��ڲ���:��     ���ڲ������� 
***********************************************************/ 
void phone()    //��绰����
{  
	 Print_Str("ATD");
     Print_Str(PhoneCall);   //���ʹ�绰ָ��+Ŀ��绰����
	 Print_Char(';');
     Print_Str("\r\n");   //���ͻس� 
     ES=1;  
} 
/*********************************************************** 
�������ƣ�main
�������ܣ�������
***********************************************************/ 
void main()
{ 
Ini_UART(); 
 	
while(1)
{  

   if(!Hang_up)  //�ҵ绰�����Ƿ���
        {
           delay(10); if(!Hang_up)  //�ҵ绰�����Ƿ���
        {
            GSM_ATH(); //���͹ҵ绰ָ�� 
			LED0=1;
            delay(2000); 
            Calls_State_Flag=Clear_Connect;   
          }}
   else if((!Phone_Call_Key)&&(Calls_State_Flag==Clear_Connect))
        {delay(10);
          if((!Phone_Call_Key)&&(Calls_State_Flag==Clear_Connect))
             {
              phone();  //����绰
               
			   LED0=0;
              delay(2000); 
              Calls_State_Flag=Phone_connection;
        	 }
		}   
		
   
}
}