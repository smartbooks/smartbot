/**********************TS-51A��Ƭ������������************************
*		Copyright (c) 2013,52̽�� ��Ƭ��������
*		All rights reserved
*       
*		�ļ����ƣ�main.c
*		����������nRF24L01���̡�
*					����ͬһ�����򵽿�����оƬ�ϣ�����K1������������ᷢ��������
*					����K2��LED1��ˮ�Ʒ���ܻ������
* 
*		��ǰ�汾��1.1
*		��    �ߣ�52̽��
*       ������ڣ�2013-5-29
*		�޸����ڣ�2013-9-15
*
*		ȡ���汾��1.0
*       �Ľ����ݣ��޸�nRF24L01�ܽŶ���
*
*		ʹ�û�����TS-51A������ + STC89C52 + 11.0592M���� + Keil C51 V9.0 
*       
*		�Ա���ַ��http://shop104208028.taobao.com/
*		QQ      : 10903659
*		QQȺ    ��336397723  336398729   336398900
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
