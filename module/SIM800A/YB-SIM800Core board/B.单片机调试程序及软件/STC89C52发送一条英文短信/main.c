#include"reg52.h"
#include "uart.c"
#include "delay.h"


uchar code PhoneNO[]= "18845146761"; //���շ�����
uchar code Text1[]= "Harmfulgases";  //����


void main()
{
	Uart_init();
	while(1)
	{
	
		SendString("AT+CMGF=1\r\n");	
		
		delayms_1000ms();
		
		SendString("AT+CSCS=\"GSM\"\r\n");	
		
		delayms_1000ms();
		
		SendString("AT+CMGS=");	//��Ϣ����ָ�� AT+CMGS=//
		SendASC('"');
		SendString(PhoneNO);   
		SendASC('"');
		SendASC('\r');			//���ͻس�ָ��//
		SendASC('\n');			//���ͻس�ָ��//
		
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

 
 


 /*���⣺
   ÿ�δ��������ݡ�RsPoint����������ģ����������� ׼����һ�����ݣ�Ȼ�����һ������ ֮ǰ
   RsBuf[RsPoint++]=SBUF;
		RsBuf[RsPoint]=0x00;   //����һ����������

		����������û���⡣��ɾ�������պ������ο������ϵ�tc35.c

		���Һ���˵���֧��	�������ֻ�
*/
 
