//*****************STC控制GSM发送短信*************************************/
//发送PDU的按键对应开发板的5-P1^7，关于PDU代码：利用我们的资料的测试软件Ggsmmoden生成
//你所要发送的短信内容，直接调入Byte_num[]	  Sms2_Pdu[] 数组中就可以的
//*************************************************************************
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
uchar code SMS_send[]= "AT+CMGS=";  
sbit Send_SMS_PDU_key=P1^7;     //发PDU格式短信按键
uchar code SMS_PDU[]= "AT+CMGF=0";//发送数据为PDU格式 
uchar Byte_num[]="32";			   //
uchar code Sms2_Pdu[]="0891683108200205F011000B815129220665F60008A7128BF76CE8610FFF0C6E295EA68FC79AD8FF01"; //内容是：您好

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
函数名称：GSM_PDU
函数功能：设置发送数据为PDU格式       
          入口参数:无     出口参数：无 
***********************************************************/ 
 
void GSM_PDU()    //设置GSM为PDU格式
 {
     Print_Str(SMS_PDU);  //发送设置指令
     Print_Str("\r\n");   //发送回车
      ES=1;
     delay(1000); 
     delay(1000);
     delay(1000);
	 delay(1000);
  }

/*********************************************************** 
函数名称：GSM_PDU_SMS
函数功能：发送数据为PDU格式的短信内容       
          入口参数:无     出口参数：无 
***********************************************************/ 
void GSM_PDU_SMS()  //发送PDU格式内容
{   
	   
     Print_Str(SMS_send);
     Print_Str(Byte_num);  
	// Print_Str(PhoneCall);
     Print_Str("\r\n"); //发送回车
	  delay(900);
     Print_Str(Sms2_Pdu);
     Print_Char(0x1a);  //发送短息内容结束符对应的ASCII为0x1a
   	   ES=1;
     delay(5000);
     delay(5000);   
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

   if(!Send_SMS_PDU_key)  //挂电话按键是否按下
        {
           GSM_PDU();
		   //delay(500);
         GSM_PDU_SMS();   
          }
  
      
}
}