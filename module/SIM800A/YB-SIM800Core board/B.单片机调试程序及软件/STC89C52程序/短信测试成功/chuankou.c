#include <reg52.h>
#include <intrins.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>  
#include<string.h> 
#define uint unsigned int
#define uchar unsigned char 
//void init(void);
//char j='\0';
char RsBuf[40];
char code AT_SK[]=">";
char code AT_Code[]="OK";
//bit flag=0;
uchar RsPoint=0;
sbit led1=P0^1;
sbit led2=P0^2;
sbit led3=P0^3;
sbit led4=P0^4;	
void Delay(uint z)
{
  uint x,y;
  for(x=z;x>0;x--)
     for(y=125;y>0;y--);

}
/***************************************8
***************************************/
 void  ClrRsBuf(char *p,int tt)
{
	  //int i;
	  while(tt--)
	  {
	  *p='\0';
	   p++;
	  }
}
//////////////////////////////////////
 void Send_AT(void)
{  uchar code mode[]="AT";
   uchar i;
   unsigned char *p;
	while(1)
	{	
    ClrRsBuf(RsBuf,sizeof(RsBuf));	
 	for(i=0;i<2;i++) 
	{
	SBUF=mode[i];	//将table[i]送给发送缓冲器				
	while(!TI);		 //等待发送完毕
	TI=0;	
	}
	SBUF='\r';
	while(!TI);		 //等待发送完毕
	TI=0;   

    ES=1;    //????
    Delay(500);
    p=strstr(RsBuf,AT_Code);
    if(p!=NULL) break;
	}
}

/*********************************************************** 
????:CMGF 
????:???????TXET??       ????:?     ????:? 
***********************************************************/ 
void Send_CMGF(void)
  { uchar i;	 
	uchar code mode[]="AT+CMGF=1";	
    unsigned char *p;
	while(1)
	{	
    ClrRsBuf(RsBuf,sizeof(RsBuf));
 	for(i=0;i<9;i++)
	{
	SBUF=mode[i];	//将table[i]送给发送缓冲器				
	while(!TI);		 //等待发送完毕
	TI=0;
 	}
	SBUF='\r';				
	while(!TI);		 //等待发送完毕
	TI=0;

	ES=1;    //????
    Delay(500);
    p=strstr(RsBuf,AT_Code);
    if(p!=NULL) break;
	  }
}
/************CSCS*********************************8
********************************************/ 
void  CSCS (void) 
{ 		   	
   	uchar i;
	uchar code mode[]="AT+CSCS=\"GSM\""; 	

 	for(i=0;i<14;i++)
	//while()
	{
	SBUF=mode[i];	//将table[i]送给发送缓冲器				
	while(!TI);		 //等待发送完毕
	TI=0;
 	}
	SBUF='\r';				
	while(!TI);		 
	TI=0;
	SBUF='\n';
	while(!TI);		 //等待发送完毕
	TI=0;
	
} 
/*********************************************************** 
????:CMGS 
????:????SIM???                 ????:?     ????:? 
***********************************************************/ 
void   Send_CMGS(void) 
{ 
    uchar i;
	uchar code phone[]="AT+CMGS=\"13548624123\"";
	unsigned char *p;
	while(1)
	{	
    ClrRsBuf(RsBuf,sizeof(RsBuf));
 	for(i=0;i<21;i++){
	SBUF=phone[i];	//将table[i]送给发送缓冲器				
	while(!TI);		 //等待发送完毕
	TI=0;
 	} 
	SBUF='\r';				
	while(!TI);		 //等待发送完毕
	TI=0;
	 	  
	ES=1;    //????
    Delay(500);
    p=strstr(RsBuf,AT_SK);
    if(p!=NULL) break;
	} 
	   
} 

void  SEND(void) 
{ 
    uchar i=0;
	uchar code sms[]="TEST OK!!\x1a ";	//sms??????????,\x01a?Ctrl+Z 
 	while(sms[i]!='\0')
	{
	SBUF=sms[i];	//将table[i]送给发送缓冲器				
	while(!TI);		 //等待发送完毕
	TI=0;
	i++;
 	}
//	SBUF='\n';
//	while(!TI);		 //等待发送完毕
//	TI=0;
} 

void init(){
	TMOD=0x20;
	TH1=0xFA;	  //bit=9600
	TL1=0xFA;
	TR1=1;
	REN=1;		  //允许接受
	SM0=0;
	SM1=1;
	EA=1;
	ES=1;		   //开串口中断
}

///////////////////////////////
/*void SendMsgStart(void)
{
unsigned char *p,i=10; 	
    ES=1;
while(i--)
{
  Delay(100);  //Get">"
  p=strstr(RsBuf,SK);  //">"
  if(p!=NULL)
  {
    ClrRsBuf(RsBuf,sizeof(RsBuf));
     Delay(150); //Get">"
	 led4=1;
    break;
	
}
}
}
	*/
////////////////////////////
void main(){    
	init();		   
	   //flag=0;
	   	led1=0;
	   	led2=0;
	   	led3=0;
	   	led4=0;
	   	Delay(50);
		ES=0;
		Delay(2000); 
		Send_AT();
		led1=0;	
        Delay(1000); 	
       		led2=0;	          
    //	Delay(1000);
    	//CSCS();
		Delay(1000); 
   	    Send_CMGS();     //????????
		led3=0; 	
		Delay(1000);
		SEND();
		led4=0; 	
	//	SendMsgStart();		
		while(1);
}

void ser()interrupt 4
{
	 if(RI==1)
  {
    RI=0;
    RsBuf[RsPoint++]=SBUF;
    if (RsPoint>=sizeof(RsBuf))
    {
    RsPoint=0;//FlagRs485=0;
    }  //????
  }
	
} 	  
