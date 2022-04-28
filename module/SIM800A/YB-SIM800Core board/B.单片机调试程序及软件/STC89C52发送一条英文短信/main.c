#include"reg52.h"
#include "uart.c"
#include "delay.h"


uchar code PhoneNO[]= "18845146761"; //接收方号码
uchar code Text1[]= "Harmfulgases";  //内容


void main()
{
	Uart_init();
	while(1)
	{
	
		SendString("AT+CMGF=1\r\n");	
		
		delayms_1000ms();
		
		SendString("AT+CSCS=\"GSM\"\r\n");	
		
		delayms_1000ms();
		
		SendString("AT+CMGS=");	//信息发送指令 AT+CMGS=//
		SendASC('"');
		SendString(PhoneNO);   
		SendASC('"');
		SendASC('\r');			//发送回车指令//
		SendASC('\n');			//发送回车指令//
		
		delayms_1000ms();
		
		SendString(Text1);  

		delayms_1000ms();
		
		SendASC(0x1a);		
		
		delayms_1000ms();
		delayms_1000ms();
		delayms_1000ms();
		delayms_1000ms();
		delayms_1000ms();
	}

 }

 
 


 /*问题：
   每次处理完数据。RsPoint是怎样清零的？？？？？在 准备发一个数据，然后接受一个数据 之前
   RsBuf[RsPoint++]=SBUF;
		RsBuf[RsPoint]=0x00;   //将下一个数据清零

		如果这个发送没问题。加删除，接收函数，参考桌面上的tc35.c

		添加液晶菜单的支持	，类似手机
*/
 
