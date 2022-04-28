#ifndef _UART_C_
#define	 _UART_C_

#include"reg52.h"

#define uchar unsigned char
#define uint unsigned int

/*
void delay_uart(uint ms)
{
	uint i,j;
	for(i=0;i<ms;i++)
		for(j=0;j<112;j++);
}    
*/
void Uart_init()
{
	
	TMOD= 0X20;		//T1  ��ʽ2  ��8λ   �Զ���װ   
	TH1=0Xfd;
	TL1=0Xfd;	    //9600
	TR1=1; 		    // ��ʱ��1����  


	SM0=0;	    // ���ô��ڵĹ���ģʽ
	SM1=1; 	    //��ʽ1


	REN=1; 		 // �����ڽ������� 	 


	ES=1; 	     // �����ж�Ӧ�� 
	EA=1;		 // �����ж� 
	
}

void SendASC(uchar d)
{
	
	SBUF=d;
	while(!TI);

	TI=0;
}
void SendString(uchar *str)	
{
	while(*str)
	{
		SendASC(*str) ;
		str++;
		//delay_uart(1);
	}
}
#endif
												       



