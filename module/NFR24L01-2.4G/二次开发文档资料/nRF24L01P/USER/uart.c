
#include "uart.h"
#include "config.h"

void InitUART(void)
{
    SCON  = 0x50;    // SCON: 模式 1, 8-bit UART, 使能接收  
    TMOD |= 0x20;    // TMOD: timer 1, mode 2, 8-bit 重装
    TH1   = 0xFD;    // TH1:  重装值 9600 波特率 晶振 11.0592MHz  
    TR1   = 1;       // TR1:  timer 1 打开                         
    EA    = 1;       // 打开总中断
}  

/*------------------------------------------------
                    发送一个字节
------------------------------------------------*/
void SendByte(unsigned char dat)
{
   SBUF = dat;
   while(!TI); TI = 0;
}

/*------------------------------------------------
                    发送一个字符串
------------------------------------------------*/
void SendStr(unsigned char *s)
{
  while(*s!='\0')  // \0 表示字符串结束标志，//通过检测是否字符串末尾
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


 