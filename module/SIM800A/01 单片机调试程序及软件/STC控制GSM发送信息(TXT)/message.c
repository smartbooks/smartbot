//*****************STC控制GSM发送短信(TXT格式)*************************************/
//发送PDU的按键对应开发板的按键接口P1^6，
//发送短信时，直接调换数组 PhoneNum	  Sms 内的内容就可以的
//*************************************************************************
#include <reg52.h>
#include <absacc.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit Send_SMS_TXT_key=P1^6; 
uchar code PhoneNum[]={"AT+CMGS=\"+8615858270262\""};
uchar code TxtMode[]={"AT+CMGF=1"};
uchar code Sms[] = {"GsmTest"};
delay(uint ms)// 延时子程序
{      
uchar i;
while(ms--)
{
  for(i = 0; i < 120; i++);
}
}
Print_Char(uchar ch)//发送单个字符
{
SBUF = ch; //送入缓冲区
while(TI == 0); //等待发送完毕
TI = 0; //软件清零
}
Print_Str(uchar * str,uint len)//发送字符串,调用Send_Char() len字符串长度
{

while(len--)
{
Print_Char(*str++);
    }

}
Ini_UART(void)//串口初始化、定时器初始化
{
TMOD = 0x20;
TH1 = 0xFD;
TL1 = 0xFD;
SCON = 0x50;
PCON = 0x00;

TF1 = 1;
TR1 = 1;
ES=1;
ET0=1;
EA=1;
}	
void GSM_TXT()
{   
     Print_Str(TxtMode,9);
     Print_Char(0X0D);
	 Print_Char(0X0A);
      ES=1;
     delay(1000); 
}
void GSM_CMGS()
{   
     Print_Str(PhoneNum,24);
	 Print_Char(0X0D);
	 Print_Char(0X0A);
     ES=1;
     delay(1000);   
}
void GSM_SMS()
{   
     Print_Str(Sms,7);
     Print_Char(0x1a);
     ES=1;
     delay(1000);
     delay(5000);   
}
void main()
{

Ini_UART();
while(1)
{ 
if(!Send_SMS_TXT_key)
{
    GSM_TXT();
    GSM_CMGS();
    GSM_SMS();  
    delay(50000);
    delay(50000);
   
  }
}
}