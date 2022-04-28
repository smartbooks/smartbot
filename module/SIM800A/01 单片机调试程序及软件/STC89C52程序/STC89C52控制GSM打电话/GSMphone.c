#include <reg52.h>
#include <absacc.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char
#define Phone_connection  1    //正在打电话状态标志
#define Clear_Connect     0    //电话已挂断状态标志
sbit Phone_Call_Key=P1^0;     //打电话按键
sbit Hang_up=P1^1;     //挂电话按键 

sbit LED0=P0^0;
sbit LED1=P0^1;
sbit LED2=P0^2;
sbit LED3=P0^3;
sbit LED4=P0^4;
sbit LED5=P0^5;
sbit LED6=P0^6;
sbit LED7=P0^7;

char Calls_State_Flag=Clear_Connect;   //电话状态  
uchar code PhoneCall[]="02228214113";
uchar code ATH[]="ATH";
void delay(uint ms)// 延时子程序
{      
uchar i;
while(ms--)
{
  for(i=0;i<120;i++);
}
}

/*********************************************************** 
函数名称：Print_Char 
函数功能：发送单个字符     
          入口参数:ch      出口参数：无 
***********************************************************/
void Print_Char(uchar ch)//发送单个字符
{
SBUF=ch; //送入缓冲区
while(TI==0); //等待发送完毕
TI=0; //软件清零
}

/*********************************************************** 
函数名称：Print_Str 
函数功能：发送字符串     
          入口参数:*str    出口参数：无 
***********************************************************/
Print_Str(uchar *str)//发送字符串
{

while(*str!='\0') 
{
Print_Char(*str++);
    }

}
/*Print_Str(uchar *str,uint len)//发送字符串,调用Send_Char() len字符串长度
{

while(len--)
{
Print_Char(*str++);
    }

}*/

 
/*********************************************************** 
函数名称：Ini_UART 
函数功能：串口初始化、定时器初始化     
          入口参数:无     出口参数：无 
***********************************************************/ 
Ini_UART(void)//串口初始化、定时器初始化
{
TMOD = 0x20;      //T1方式2,用于UART波特率 
TH1 = 0xFD;       //UART波特率设置:9600 
TL1 = 0xFD;
SCON = 0x50;  ////UART方式1:8位UART;   REN=1:允许接收 
PCON = 0x00;

TF1 = 1;     // 中断标志位
TR1 = 1;    // 启动定时器1    
ES=1;       //启动串行口中断
ET0=1;      //启动中断功能
EA=1;
}
 
/*********************************************************** 
函数名称：GSM_ATH 
函数功能：挂电话      
          入口参数:无     出口参数：无 
***********************************************************/ 
void GSM_ATH()   
{   
     Print_Str(ATH); //发送挂电话指令
     Print_Str("\r\n");   //发送回车
      
     //delay(1000); 
     //delay(1000); 
}  
	
/*********************************************************** 
函数名称：phone 
函数功能：拨打目标电话      
          入口参数:无     出口参数：无 
***********************************************************/ 
void phone()    //打电话函数
{  
	 Print_Str("ATD");
     Print_Str(PhoneCall);   //发送打电话指令+目标电话号码
	 Print_Char(';');
     Print_Str("\r\n");   //发送回车 
     ES=1;  
} 
/*********************************************************** 
函数名称：main
函数功能：主函数
***********************************************************/ 
void main()
{ 
Ini_UART(); 
 	
while(1)
{  

   if(!Hang_up)  //挂电话按键是否按下
        {
           delay(10); if(!Hang_up)  //挂电话按键是否按下
        {
            GSM_ATH(); //发送挂电话指令 
			LED0=1;
            delay(2000); 
            Calls_State_Flag=Clear_Connect;   
          }}
   else if((!Phone_Call_Key)&&(Calls_State_Flag==Clear_Connect))
        {delay(10);
          if((!Phone_Call_Key)&&(Calls_State_Flag==Clear_Connect))
             {
              phone();  //拨打电话
               
			   LED0=0;
              delay(2000); 
              Calls_State_Flag=Phone_connection;
        	 }
		}   
		
   
}
}