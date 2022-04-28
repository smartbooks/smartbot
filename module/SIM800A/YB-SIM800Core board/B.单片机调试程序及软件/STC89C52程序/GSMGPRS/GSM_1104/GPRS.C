#include<reg52.h>
#include<absacc.h>
#include<stdio.h>

#include"ds1302.h"
#include"ds18b20.h"
#include"1602.h"

#define uint unsigned int
#define uchar unsigned char
#define RST      0X01
#define CMGF     0X02
#define CMGD     0X03
#define CREG     0X04
#define CMGR     0X05
#define CMGS     0X06
#define CSCA     0X07
#define ATDAT    0X08
#define TC35RET  0X00
sbit IGT=P1^3;
uchar a,b,c,d,C_TEMP;
uint temp;

int sum=0,sum1=0;
//uchar code tab[]={0x03,0x9f,0x25,0x0d,0x99,0x49,0x41,0x1f,0x01,0x09,0xfd,0xff};//0-9,-,全灭 (共阳字段表)
uchar str0[16]={"TIME 00:00:00   "};
uchar str1[16]={"TEMP 000.0C     "};

uchar code   GSM_START[16]={"GSM START!      "};
uchar code  GSM_START1[16]={"GSM START!!     "};
uchar code  GSM_START2[16]={"GSM START!!!    "};
uchar code      ATE_OK[16]={"ATE0 OK!        "};
uchar code   ATCMGF_OK[16]={"TEXT MODE OK!   "};
uchar code  ATCREG1_OK[16]={"NET CONNECT OK! "};
uchar code  ATCREG2_OK[16]={"NET CONNECTING! "};
uchar code   ATCSCA_OK[16]={"CENTER NUM OK!  "};
uchar code   ATCMGS_OK[16]={"PHONE NUM OK!   "};
uchar code    ATGSM_OK[16]={"GSM SEND OK!    "};
uchar code    ATGSM_FL[16]={"GSM SEND RETRY! "};
#include"GSM.h"
void initLCM( void);               											//LCD初始化子程序
void lcd_wait(void);              											//LCD检测忙子程序
void WriteCommandLCM(uchar WCLCM,uchar BusyC);                              //写指令到ICM子函数
void WriteDataLCM(uchar WDLCM);                                             //写数据到LCM子函数
void DisplayOneChar(uchar X,uchar Y,uchar DData);                           //显示指定坐标的一个字符子函数
void DisplayListChar(uchar X,uchar Y,uchar *DData);                    //显示指定坐标的一串字符子函数

void ATE0(void);
void delay_GSM(uint);
void init_serialcom( void );
void send_uart( unsigned char);
void ATNAME(void);
void AT_CMGS(void);
void AT_CSCA(void);
void AT_RST(void);
void AT_CREG(void);
void AT_CMGF(void);
void AT_CMGR(void);
void AT_CMGD(void);
unsigned int ReadTemperature(void);

void Init_T0 (void)   
{           
    TMOD = 0x21;   
    TH0 = (65536-10000) /256; 
    TL0 = (65536-10000) %256;
    EA = 1;      
    ET0 = 1;
    TR0 = 1;      
}
void Time0_ISR(void)  interrupt 1    using 0
{  
 TH0 = (65536-10000) /256; //初始值
 TL0 = (65536-10000) %256; 
C_TEMP++;

}
//显示时间和温度
void LCD_SCREEN()
{if(C_TEMP>=90){
temp=readtemperature();
b=ReadTimeValue(0X81);
c=ReadTimeValue(0X83);
d=ReadTimeValue(0X85);
str0[5]=d/0x10+0x30;
str0[6]=d%0x10+0x30;
str0[8]=c/0x10+0x30;
str0[9]=c%0x10+0x30;
str0[11]=b/0x10+0x30;
str0[12]=b%0x10+0x30;
a=(temp-(temp&10))/10;
str1[5]=(a/100)+0x30;
str1[6]=(a/10)+0x30;
str1[7]=(a%10)+0x30;
str1[9]=(temp%10)+0x30;
C_TEMP=0;
//LCD(str0,str1);
}
}
/*
uchar code     ATE_OK[16]={"ATE0 OK!        "};
uchar code  ATCMGF_OK[16]={"TEXT MODE OK!   "};
uchar code ATCREG1_OK[16]={"NET CONNECT OK! "};
uchar code ATCREG2_OK[16]={"NET CONNECTING! "};
*/



main()
{//P1=0X00;
//--------初始化-------------------
init_serialcom(); 
Init_T0();
InitDS1302();
//-----------------------------
LCD(str0,GSM_START);
delay_GSM(100);
IGT=0;
delay_GSM(100);
IGT=1; 
LCD(str0, GSM_START1);
delay_GSM(100);
TC35_MAX=0;
ATE0();
delay_GSM(100);
LCD(str0, GSM_START2);
for(i_TC35=0;i_TC35<=29;i_TC35++)
{TC35_RE[i_TC35]=0;}
i_TC35_RECE=0;
TC35_MAX=0;
ATE0();
while(1)
{
//delay_GSM(200);
//LCD(str0, ATCREG2_OK);
LCD_SCREEN(); 
SEND_MESSAGE();
}
}


