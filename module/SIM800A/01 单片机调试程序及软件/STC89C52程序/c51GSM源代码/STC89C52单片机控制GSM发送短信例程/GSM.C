#include <reg52.h>
#include <intrins.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>  
#include<string.h> 
#define uint unsigned int
#define uchar unsigned char	
		 
sbit Send_SMS_Text_key=P1^0;     //发Text格式短信按键
sbit Send_SMS_PDU_key=P1^1;     //发PDU格式短信按键	

sbit LED0=P0^0;
sbit LED1=P0^1;
sbit LED2=P0^2;
sbit LED3=P0^3;
sbit LED4=P0^4;
sbit LED5=P0^5;
sbit LED6=P0^6;
sbit LED7=P0^7;
    	   		  
uchar code PhoneCall[]="15922260566";	
uchar code SMS_send[]= "AT+CMGS=";  
uchar code SMS_TXET[]= "AT+CMGF=1";//发送数据为TXET格式 
uchar code SMS_PDU[]= "AT+CMGF=0";//发送数据为PDU格式 
 				   		  
uchar code Sms[]="test ok!!"; 
uchar code Byte_num[]="20";  //短信内容字节长度
uchar code Sms2_Pdu[]="0891683108200205F011000B815129220665F60008A70600204F60597D"; //(5129220665F6)括号里的数为接收短信的手机号码 内容是：您好
delay(uint ms)// 延时子程序
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
Print_Char(uchar ch)//发送单个字符
{
SBUF=ch; //送入缓冲区
while(TI!=1); //等待发送完毕
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
Print_Char(*str);
   str++;
    }

}	    

 
/*********************************************************** 
函数名称：Ini_UART 
函数功能：串口初始化、定时器初始化     
          入口参数:无     出口参数：无 
***********************************************************/ 
Ini_UART(void)//串口初始化、定时器初始化
{
         SCON = 0x50 ;  //SCON: serail mode 1, 8-bit UART, enable ucvr   
                         //UART为模式1，8位数据，允许接收
          TMOD |= 0x21 ; //TMOD: timer 1, mode 2, 8-bit reload             
                         //定时器1为模式2,8位自动重装
          PCON |= 0x80 ; //SMOD=1; 
          TH1 = 0xFD ;   //Baud:19200 fosc="11".0592MHz 
          IE |= 0x90 ;     //Enable Serial Interrupt 
          TR1 = 1 ;       // timer 1 run 
          TI=1;  
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
     delay(3000); 
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
     delay(3000); 
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
     Print_Str("\r\n");   //发送回车
       delay(3000); 
     Print_Str(Sms2_Pdu);
     //Print_Str(Sms);
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
 if(!Send_SMS_Text_key) 
        {	
		  GSM_TEXT();
		  GSM_TEXT_SMS();
          LED0=!LED0;						   			 
         }
    
     else if(!Send_SMS_PDU_key)
        {	
		   GSM_PDU();
		   GSM_PDU_SMS();
           	LED1=!LED1;							   
         }
      
}
}