#include "nRF24L01P.h"
#include "delay.h"

//按键及指示灯管脚定义
#define LED3		P16  //L_LED
#define LED4		P35  //R_LED
#define LED5		P02  //STATE_LED
#define KEY1		P30	 //KEY_LEFT
#define KEY2		P31	 //KEY_RIGHT


//函数功能：检测24L01是否存在
//检测24L01是否存在，返回值:0存在 1不存在
char NRF24L01_Check(){

  char check_in_buf[5]={0x11,0x22,0x33,0x44,0x55};

  char check_out_buf[5]={0x00};

  CE=0;

  nRF24L01P_Write_Buf(WRITE_REG+TX_ADDR,check_in_buf,TX_ADR_WIDTH);

  nRF24L01P_Read_Buf(READ_REG+TX_ADDR,check_out_buf,5);

  if((check_out_buf[0] == 0x11) &&
     (check_out_buf[1] == 0x22) &&
	 (check_out_buf[2] == 0x33) &&
	 (check_out_buf[3] == 0x44) &&
	 (check_out_buf[4] == 0x55) )
	return 0;
  else
    return 1;
}

void main(void)
{
	KEY1 = 1;
	KEY2 = 1;
	
	//上电开灯自检
	LED3 = 1;
	LED4 = 1;
	LED5 = 1;
	delay_ms(1000);

	//设置为默认关闭状态
	LED3 = 0;
	LED4 = 0;
	
	//检测NRF24L01是否存在，返回值：0存在，1不存在
	while(NRF24L01_Check() == 1) {
	     LED3=1; LED4=1; LED5=1;
	     delay_ms(100);
		 LED3=0; LED4=0; LED5=0;
		 delay_ms(100);
	}

	nRF24L01P_Init();
	nRF24L01P_RX_Mode();

	while(1)
	{   

		u8 buf[32] = {0};
		KEY1 = 1;
		KEY2 = 1;

		LED5=~LED5;
		delay_ms(250);

		if(!KEY1 || !KEY2)
		{
			delay_ms(10);
			if(!KEY1)
			{
			    LED5 = 1;

				buf[0] = 0x55;
				nRF24L01P_TX_Mode();
				nRF24L01P_TxPacket(buf);
				delay_ms(200);

				LED5 = 0;
			}
			if(!KEY2)
			{
			    LED5 = 1;

				buf[0] = 0xAA;
				nRF24L01P_TX_Mode();
				nRF24L01P_TxPacket(buf);
				delay_ms(200);

				LED5 = 0;
			}
			buf[0] = 0;
			nRF24L01P_RX_Mode();
		}
		
		
		if(!nRF24L01P_RxPacket(buf))
		{
			switch(buf[0])
			{
				case 0x55:
					LED3 = 0;
					delay_ms(100);
					LED3 = 1;
					delay_ms(100);
					break;
				case 0xAA:
					LED4 = 0;
					delay_ms(100);
					LED4 = 1;
					delay_ms(100);
					break;
				default:
					break;
			}
			buf[0] = 0;	
		}
	}
}
