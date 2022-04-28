/********************************************************
历程描述：GSM模块联网成功后，启动单片机，单片机发出“滴”声
后，开始控制GSM模块发送一条中文短信（龙程电子欢迎您！123）
。发送完毕后。单片机发出“滴”声。例程结束。
********************************************************/
#include<reg52.h>
#include<intrins.h>
#include<stdlib.h>
#define uchar unsigned char
#define uint unsigned int
sbit FMQ=P3^6;	//蜂鸣器 
uchar code at[]="AT\r";
uchar code TC_MSXZ[]={"AT+CMGF=0\r"};//	发送模式选择为PDU，可以发送中文字符
uchar code TC_MDSJ[]={"AT+CMGS=37\r"}; // 信息长度
uchar code TC_FSNR[]={"0891683108200905F011000D91685191418135F80008AA169F997A0B75355B506B228FCE60A80021003100320033"};

uchar num,temp,key,m=3,n=0,z=0;
void delay_1ms(uint z) //延时函数
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}
/****************  数据发送 *****************/
 void send(uchar *tab)
 {
 	while((*tab)!='\0')
	{
	//	ES=0; //关串口中断
		SBUF=*tab;
		while(TI==0);
		TI=0;
		tab++;
	//	ES=1;  //开串口中断
	}
 }
/********** 串口初始化 ***********/
 void uart_int(void)
 {
 	SCON=0x50;//UART方式1：8位UART；
	REN =1;	  //允许串行口接收数据；
	PCON=0x00;//SMOD=0：波特率不加倍；
	TMOD=0x20;//T1方式2，用于UART波特率；
	TH1 =0xFD; //装初值；
	TL1 =0xFD; //装初值
	TR1 =1;	   //启动定时器1
//	EA=1;   //打开全局中断控制
//	ES=1;    //打开串行口中断
 }
void fengmingqi() //蜂鸣器
{	uint b,r;
	for (b=0;b<300;b++)
	{
		FMQ=0;
		for(r=50;r>0;r--);
		FMQ=1;
		for(r=50;r>0;r--);	
	}	
}
void main()
{
/*	DLED=1;
	P0=0x00;
	DLED=0;
	n=0;
	init();	*/
	delay_1ms(1000);
	fengmingqi();  //发出“滴”声
	uart_int();//串口初始化
	send(at);  //发送AT
	delay_1ms(500);
	
	
	send(TC_MSXZ); //短信模式选择
	delay_1ms(500);
	send(TC_MDSJ); //信息长度
	delay_1ms(500);
	send(TC_FSNR); //发送内容
	delay_1ms(500);
	SBUF=0x1A;	  //16进制发送1A
	
	
	while(TI==0);
	TI=0;
	fengmingqi(); //发出“滴”声
	delay_1ms(500);
	while(1);	  //程序结束
}