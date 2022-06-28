
#include <STC15F2K60S2.H>
#include <stdio.h>
#include "uart.h"

void uartSendData(char dat)
{
    ES = 0;

	SBUF = dat;

	while(TI!=1);

	TI=0;

	ES = 1;
}

char putchar(char c)
{
	uartSendData(c);
	    
    return c;
}

void InitUART()
{
	SCON  = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器时钟1T模式
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TL1   = 0xE0;		//设置定时初始值
	TH1   = 0xFE;		//设置定时初始值
	ET1   = 0;		    //禁止定时器中断
	TR1   = 1;		    //定时器1开始计时

	OpenUARTReceive();
}  

void OpenUARTReceive(){
	//串口接收配置
	EA	  = 1;          //先打开总中断
	ES    = 1;          //再打开串口接收中断
}

void CloseUARTReceive(){
	//串口关闭接收配置
	ES    = 0;          //关闭串口接收中断
	EA	  = 0;          //关闭总中断	
}

void SendByte(unsigned char textByte)
{
   SBUF = textByte;
   
   while(!TI); 
   
   TI = 0;
}


void SendToUART(unsigned char *text)
{
  while(*text != '\0')
  {
    SendByte(*text);
	
    text++;
  }
}

void log(unsigned char *text){

	printf("%s\r\n", text);

}

void ReceiveUartData(void) interrupt 4   //串行中断服务程序
{
	//定义临时变量
	unsigned char tempReceiveData;
	
	//判断是接收中断产生
	if(RI)
	{
		RI=0;                    //标志位清零

		tempReceiveData = SBUF;  //读入缓冲区的值

		SBUF = tempReceiveData;  //把接收到的值再发回电脑端
	}

    //如果是发送标志位，清零
	if(TI)
	{
		TI=0;
	}
} 