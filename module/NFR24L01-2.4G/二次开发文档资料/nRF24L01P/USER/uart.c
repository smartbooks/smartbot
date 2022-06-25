
#include "uart.h"
#include "config.h"

void InitUART(void)
{
    SCON  = 0x50;    // SCON: ģʽ 1, 8-bit UART, ʹ�ܽ���  
    TMOD |= 0x20;    // TMOD: timer 1, mode 2, 8-bit ��װ
    TH1   = 0xFD;    // TH1:  ��װֵ 9600 ������ ���� 11.0592MHz  
    TR1   = 1;       // TR1:  timer 1 ��                         
    EA    = 1;       // �����ж�
}  

/*------------------------------------------------
                    ����һ���ֽ�
------------------------------------------------*/
void SendByte(unsigned char dat)
{
   SBUF = dat;
   while(!TI); TI = 0;
}

/*------------------------------------------------
                    ����һ���ַ���
------------------------------------------------*/
void SendStr(unsigned char *s)
{
  while(*s!='\0')  // \0 ��ʾ�ַ���������־��//ͨ������Ƿ��ַ���ĩβ
  {
    SendByte(*s);
    s++;
  }
}

/*
void UART_SER(void) interrupt 1
{
   //unsigned char Temp;

   if(RI)
   {
      RI=0;

	  P00 = ~P00;
	  //Temp=SBUF;
	  //P1=Temp;
      //SBUF=Temp;
   }

   if(TI) TI=0;
}
*/

void UartTestInit(){
    TMOD = 0X20;
	SM0 = 0;
	SM1 = 1;
	REN = 1;
	PCON = 0;
	TH1 = 0xFD;
	TL1 = 0xFD;
	TR1 =1;
}

void UartTest(){
	P01  = SBUF; while(!RI); RI=0;
	SBUF = 'A' ; while(!TI); TI=0;
}


 