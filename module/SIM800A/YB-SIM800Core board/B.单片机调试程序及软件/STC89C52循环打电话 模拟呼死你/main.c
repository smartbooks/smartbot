#include"reg52.h"
#include "uart.c"
#include "tc35.c"


void main()
{
	Uart_init();
	while(1)
	{
	
		SendString("ATD18845146761;\r\n");	//����ĳ�����ֻ����룬ATDXXXXXXXXXX; XXXXXXXXXX�滻����ֻ���
		
		
		delayms_1000ms();
		delayms_1000ms();
		delayms_1000ms();
		delayms_1000ms();
		delayms_1000ms();
		
		SendString("ATH\r\n");
		
				
		delayms_1000ms();
		delayms_1000ms();
		delayms_1000ms();
		delayms_1000ms();
		delayms_1000ms();	
	}

 }
