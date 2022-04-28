#include<reg52.h>
#include<absacc.h>
#include<stdio.h>
#include"GSM.h"  

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
uchar a,b,c,d;//,C_TEMP
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
TC35_MAX=0;
ATE0();
delay_GSM(100); 
for(i_TC35=0;i_TC35<=29;i_TC35++)
{TC35_RE[i_TC35]=0;}
i_TC35_RECE=0;
TC35_MAX=0;
ATE0();
while(1)
{
//delay_GSM(200); 
SEND_MESSAGE();
}
}


