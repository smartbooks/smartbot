/**********************TS-51A单片机开发板例程************************
*		Copyright (c) 2013,52探索 单片机开发板
*		All rights reserved
*       
*		文件名称：main.c
*		功能描述：nRF24L01例程。
*					下载同一个程序到开发板芯片上，按下K1，二个开发板会发出声音。
*					按下K2，LED1流水灯发光管会点亮。
* 
*		当前版本：1.1
*		作    者：52探索
*       完成日期：2013-5-29
*		修改日期：2013-9-15
*
*		取代版本：1.0
*       改进内容：修改nRF24L01管脚定义
*
*		使用环境：TS-51A开发板 + STC89C52 + 11.0592M晶振 + Keil C51 V9.0 
*       
*		淘宝地址：http://shop104208028.taobao.com/
*		QQ      : 10903659
*		QQ群    ：336397723  336398729   336398900
******************************************************************/

#include ".\\nRF24L01\\nRF24L01.h"
#include "def.h"

void Delay(unsigned int s)
{
	unsigned int i;
	for(i=0; i<s; i++);
	for(i=0; i<s; i++);
}


void main(void)
{
	unsigned char leng =0;
	unsigned char tf =0;
	unsigned char TxBuf[20]={0};	 // 
	unsigned char RxBuf[20]={0};	

    init_io() ;

	BEEP = 0;
	LED2 = 0;
	//TxBuf[0] = 8 ;
	TxBuf[1] = 1 ;
	TxBuf[2] = 1 ;
	nRF24L01_TxPacket(TxBuf);	// Transmit Tx buffer data
	Delay(6000);
	BEEP = 1;
    LED2 = 1;

	while(1)
	{
	    //SetRX_Mode();
    	if(K1 ==0 ) 
	  	{
		   	BEEP = 0; 
		    TxBuf[1] = 1 ;
		    tf = 1 ; 
			//while(KEY1 ==0);
	    }
	   if(K2 ==0 )
	   {
			LED2 = 0;
			TxBuf[2] = 1 ;
			tf = 1 ; 
			//while(KEY2 ==0);
	   }

	   if (tf==1)
       {	
			nRF24L01_TxPacket(TxBuf);	// Transmit Tx buffer data
			TxBuf[1] = 0x00;
			TxBuf[2] = 0x00;
			tf=0;
			Delay(1000);
			BEEP = 1;
			LED2 = 1;

			//setRxMode();
	   }

		SetRX_Mode();
   		if(nRF24L01_RxPacket(RxBuf))
		{
						
			if(	RxBuf[1]==1)
			{	 	
				BEEP = 0;
			}
			if(	RxBuf[2]==1)
			{
				LED2 = 0;
			}
			Delay(1000);
			BEEP = 1;
			LED2 = 1;
		}

		RxBuf[1] = 0x00;
		RxBuf[2] = 0x00;
	}
	
}
