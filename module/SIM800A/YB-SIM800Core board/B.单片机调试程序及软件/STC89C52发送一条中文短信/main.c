#include"reg52.h"
#include "uart.c"
#include "tc35.c"

//#define RsBuf_N			50

//char RsBuf[RsBuf_N];	//���崮�ڽ������ݻ�����
//uchar RsPoint=0;


sbit huang=P2^6;
sbit hong=P2^5;
sbit lv=P2^4;
sbit buzz=P2^2;
sbit jk=P2^1;

uchar code PhoneNO[]= "00310038003800340035003100340036003700360031"; //���ܺ��룬����ת����
uchar code Text1[]= "4F60731C6211662F8C01FF1F"; //�������ݣ�����ת����




uchar code center[]="+8613800451500";

void main()
{
	Uart_init();
	
	SendString("AT+CMGF=1\r\n");	
	
	delayms_1000ms();
	
	SendString("AT+CSCS=\"UCS2\"\r\n");	
	
	delayms_1000ms();
	
	SendString("AT+CSMP=17,0,2,25\r\n");	
	
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
	


	while(1);	

 }

 
 


 /*���⣺
   ÿ�δ��������ݡ�RsPoint����������ģ����������� ׼����һ�����ݣ�Ȼ�����һ������ ֮ǰ
   RsBuf[RsPoint++]=SBUF;
		RsBuf[RsPoint]=0x00;   //����һ����������

		����������û���⡣��ɾ�������պ������ο������ϵ�tc35.c

		���Һ���˵���֧��	�������ֻ�
*/
 
