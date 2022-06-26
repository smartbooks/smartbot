
//�����ͺ�89��15
#define mcu 89

//��������
#if (mcu==15)
	//STC15ϵ��ͷ�ļ�
	#include <STC15F2K60S2.H>

#elif (mcu==89)

	//STC89ϵ��ͷ�ļ�
	#include <reg52.h>
#endif

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
	SCON  = 0x50;		//8λ����,�ɱ䲨����

#if (mcu==15)

	//STC15ϵ�д��ڳ�ʼ��
	AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1   = 0xE0;		//���ö�ʱ��ʼֵ
	TH1   = 0xFE;		//���ö�ʱ��ʼֵ
	ET1   = 0;		    //��ֹ��ʱ���ж�

#elif (mcu==89)

	//STC89ϵ�д��ڳ�ʼ��
	TMOD |= 0x20;       // TMOD: timer 1, mode 2, 8-bit ��װ
    TH1   = 0xFD;       // TH1:  ��װֵ 9600 ������ ���� 11.0592MHz

#endif

    TR1   = 1;		    //��ʱ��1��ʼ��ʱ
	
	OpenUARTReceive();
}  

void OpenUARTReceive(){
	//���ڽ�������
	EA	  = 1;          //�ȴ����ж�
	ES    = 1;          //�ٴ򿪴��ڽ����ж�
}

void CloseUARTReceive(){
	//���ڹرս�������
	ES    = 0;          //�رմ��ڽ����ж�
	EA	  = 0;          //�ر����ж�	
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

void ReceiveUartData(void) interrupt 4   //�����жϷ������
{
	//������ʱ����
	unsigned char tempReceiveData;
	
	//�ж��ǽ����жϲ���
	if(RI)
	{
		RI=0;                    //��־λ����

		tempReceiveData = SBUF;  //���뻺������ֵ

		SBUF = tempReceiveData;  //�ѽ��յ���ֵ�ٷ��ص��Զ�
	}

    //����Ƿ��ͱ�־λ������
	if(TI)
	{
		TI=0;
	}
} 