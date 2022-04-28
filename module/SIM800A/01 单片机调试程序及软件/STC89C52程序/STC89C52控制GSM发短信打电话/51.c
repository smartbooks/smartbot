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
sbit Send_SMS_Text_key=P1^2;     //发Text格式短信按键
sbit Send_SMS_PDU_key=P1^3;     //发PDU格式短信按键

char Calls_State_Flag=Clear_Connect;   //电话状态  
uchar code PhoneCall[]="15922260566";
uchar code PhoneCall2[]="02228214113";
uchar code SMS_send[]= "AT+CMGS=";  
uchar code ATH[]="ATH";
uchar code SMS_TXET[]= "AT+CMGF=1";//发送数据为TXET格式 
uchar code SMS_PDU[]= "AT+CMGF=0";//发送数据为PDU格式 
uchar code Sms[]="test ok!!";
uchar Byte_num[]="20";
uchar code Sms2_Pdu[]="0891683108200205F011000B815129220665F60008A70600204F60597D"; //内容是：您好

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
      
     delay(1000); 
     delay(1000); 
}  
	
/*********************************************************** 
函数名称：phone 
函数功能：拨打目标电话      
          入口参数:无     出口参数：无 
***********************************************************/ 
void phone()    //打电话函数
{  
	 Print_Str("ATD");
     Print_Str(PhoneCall2);   //发送打电话指令+目标电话号码
	 Print_Char(';');
     Print_Str("\r\n");   //发送回车
     delay(5000);
     delay(5000);
     
} 

/*********************************************************** 
函数名称：GSM_TEXT 
函数功能：设置发送数据为TXET格式       
          入口参数:无     出口参数：无 
***********************************************************/ 
 
void GSM_TEXT()   //设置GSM为TEXT格式
{   
     Print_Str(SMS_TXET);   //发送设置指令
     Print_Str("\r\n");   //发送回车
      ES=1;
     delay(1000);
     delay(1000);
     delay(1000); 
}

/*********************************************************** 
函数名称：GSM_TEXT_SMS 
函数功能：发送数据为TXET格式的短信内容       
          入口参数:无     出口参数：无 
***********************************************************/ 
 
void GSM_TEXT_SMS()  //发送TEXT格式内容
{   

     Print_Str(SMS_send);
	 Print_Str(PhoneCall);
     Print_Str("\r\n");   //发送回车
     Print_Str(Sms);
     Print_Char(0x1a);  //发送短息内容结束符对应的ASCII为0x1a
     ES=1;
     delay(1000);
     delay(5000);   
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
     Print_Str("\r\n");   //发送回车
     Print_Str(Sms2_Pdu);
     Print_Char(0x1a);  //发送短息内容结束符对应的ASCII为0x1a
     ES=1;
     delay(1000);
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

   if(!Hang_up)  //挂电话按键是否按下
        {
            GSM_ATH(); //发送挂电话指令
            Calls_State_Flag=Clear_Connect;   
          }
   else if((!Phone_Call_Key)&&(Calls_State_Flag==Clear_Connect))
        {
          phone();  //拨打电话
         Calls_State_Flag=Phone_connection;
         }
    
     else if((!Send_SMS_Text_key)&&(Calls_State_Flag==Clear_Connect))
        {
          GSM_TEXT(); 	
		  GSM_TEXT_SMS();					 
         }
    
     else if((!Send_SMS_PDU_key)&&(Calls_State_Flag==Clear_Connect))
        {
         GSM_PDU();
         GSM_PDU_SMS();
         }
      
}
}