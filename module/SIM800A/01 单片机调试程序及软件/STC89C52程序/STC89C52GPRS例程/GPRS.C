#include <reg52.h>
#include <all.h>
#include <intrins.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>  
#include<string.h> 
#define uint unsigned int
#define uchar unsigned char	
	 

sbit LED0=P0^0;
sbit LED1=P0^1;
sbit LED2=P0^2;
sbit LED3=P0^3;
sbit LED4=P0^4;
sbit GPRSOK=P0^5;  //GPRS连接成功指示灯 亮：成功；灭：不成功
sbit LED6=P0^6;
sbit LED7=P0^7;	 
 
#define LenRxBuf 50
uchar GPRSBuff[30]={0}; 
char code AT_SK[]=">";
char code AT_Code[]="OK";

char idata  RS_Sets_Cont=0;    //发送  数组指针

char idata RsPoint=0;  //发送  数组指针
char idata READRsPoint=0;  //读取  数组指针
char RsBuf[LenRxBuf];     //接收缓存
//－－－定时用－－－－－－－－－－－
uint  data timercount;		//50MS定时器,
uint  data timer_S_cnt;


    	   		  
 
Delay(uint ms)// 延时子程序
{      
uchar i;
while(ms--)
{
  for(i=0;i<120;i++);
}
}

/*********************************************************** 
函数名称：Print_Char 
函数功能：发送单个字符     
          入口参数:ch      出口参数：无 
***********************************************************/
Print_Char(uchar ch)//发送单个字符
{
SBUF=ch; //送入缓冲区
while(TI!=1); //等待发送完毕
TI=0; //软件清零
}

/*********************************************************** 
函数名称：Print_Str 
函数功能：发送字符串     
          入口参数:*str    出口参数：无 
***********************************************************/
Print_Str(uchar *str)//发送字符串
{

while(*str!='\0') 
{
Print_Char(*str);
   str++;
    }

}	    


void  ClrRsBuf(char *p,int tt) //清空
{
	  //int i;
	  while(tt--)
	  {
	  *p='\0';
	   p++;
	  }
} 

/*********************************************************** 
函数名称：Ini_UART 
函数功能：串口初始化、定时器初始化     
          入口参数:无     出口参数：无 
***********************************************************/ 
Ini_UART(void)//串口初始化、定时器初始化
{
         SCON = 0x50 ;  //SCON: serail mode 1, 8-bit UART, enable ucvr   
                         //UART为模式1，8位数据，允许接收
          TMOD |= 0x21 ; //TMOD: timer 1, mode 2, 8-bit reload             
                         //定时器1为模式2,8位自动重装
          PCON |= 0x80 ; //SMOD=1; 
          TH1 = 0xFD ;   //Baud:19200 fosc="11".0592MHz 
          IE |= 0x90 ;     //Enable Serial Interrupt 
          TR1 = 1 ;       // timer 1 run 
          TI=1;  

     TH0 = 0xB8;
	 TL0 = 0x00; //20ms的时钟基准
	TR0=1;

} 
  

//////////////////////////////////////
 void Send_AT(void)
{  uchar code mode[]="AT\r";
   
   unsigned char *p;
     timercount=50;
	while(1)
	{	
//      ClrRsBuf(RsBuf,sizeof(RsBuf));	
     Print_Str(mode);
     ES=1;    // 
    Delay(500);
//    p=strstr(RsBuf,AT_Code);
    if(p!=NULL) break;
    if(timercount==0) break;
	}
}

///////////////////////////////////////////////////
//////////////////////////////////////////////////////
   
   
uchar data system_server ;    //系统状态
 
 

 /*********************************************************************
 *                  C51中字符串函数的扩充                            *
 ** 函数名称    : strsearch ()
 ** 函数功能    : 在指定的数组里连续找到相同的内容
 ** 入口参数   ： ptr2=要找的内容, ptr1 当前数组
 **** 出口参数 ： 0-没有 找到   >1 查找到
 *********************************************************************/



uchar strsearch(uchar *ptr2,uchar *ptr1_at,uchar num)//查字符串*ptr2在*ptr1中的位置
//本函数是用来检查字符串*ptr2是否完全包含在*ptr1中
//返回:  0  没有找到
//       1-255 从第N个字符开始相同
{
//、	uchar max_length;
	uchar i,j,k; 
	if(ptr2[0]==0) return(0); 
	for(i=0,j=0;i<num;i++)
	{
        if(ptr1_at[i]==ptr2[j])
        {//第一个字符相同
        	for(k=i;k<num;k++,j++)
        	{
        		if(ptr2[j]==0)//比较正确
        			return(i+1);               //返回值是整数，不含0
        		if(ptr1_at[k]!=ptr2[j]) break;
        	}
        	j=0;
        }
	}
	return(0);
}

/*********************************************************************
 *                  C51中字符串函数的扩充                            *
 ** 函数名称    : strsearch1()
 ** 函数功能    : 在指定的数组里连续找到相同的内容
 ** 入口参数   ： ptr2=要找的内容, ptr1 当前数组, id 数组开始的第一个位置
 **** 出口参数 ： 0-没有 找到   >1 查找到
 *********************************************************************/
//------在指定移态的数组里连续找到相同的内容---------------------------------------
//i=传来的指令,  *ptr2=要找的内容, uchar *ptr1 当前数组
uchar strsearch1(uchar i,uchar *ptr2,uchar *ptr1)
{      uchar j=0,k;
			k=i;
      do{
			if(ptr2[j]==0)  return(k);
       		if(ptr1[k]==ptr2[j])
       		{j++;k++;}
       		else return(0);

        }while(j<=60);
        return(0xff);
}


uchar Mygetch(char *ch)//从串口缓冲区读1字节数据
{
	 
	if(RsPoint==READRsPoint) {return 0;}   	//RxBuf Empty
	*ch=RsBuf[READRsPoint];;  READRsPoint++;					//数据被读走，读指针加1
	if(READRsPoint>=LenRxBuf) READRsPoint=0;	//如果读指针到缓冲区尾部，则返回到头部
 	return 1;
}
 
uchar MygetSets(char *Sets)//从串口缓冲区读取一个命令
{
  char a[1]={0};
if(RS_Sets_Cont)
 {
  while(1)
  {
      if(Mygetch(a)==0)
       { RS_Sets_Cont=0; return 1; }
   if(a[0]==0x0A)
    { 
    RS_Sets_Cont--;
    *Sets=0;
    return 1;
    }
   if(a[0]==0x0D)
   {
     ;
   }
   else
   {
    *Sets++=a[0];
   }
 
  }
 }
else
 {
 return 0;
 }
} 
 
//----------------------------------------------
uchar ReadBsbuffStrstr(uchar *buff,uchar *str) 
   {
      timercount=10;
      while(timercount>0)
       {
 			MygetSets(buff);
        	if(strstr(buff,str)!=NULL)
              {
						
 				 while(RS_Sets_Cont>0)
                   {
					 MygetSets(buff);	
                    }
			      return TRUE;
				}
			else if(RS_Sets_Cont==0) return FALSE;
		 
        }
	}

 
 //======================================================
//这里为AT指令处理区,所有的AT指令都在这时对uart_buff数组进行赋值,并发送出去,
//-正常情况下,AT指令反回也会在这里接收完
  //------------------------
  //-----------------------
  ///==========================================
/*
*********************************************************************************************************
** 函数名称 	 ：Send_AT_Command()
** 函数功能		 ：串口AT指令发送前处理,
** 全局变量或数组:   receive_count 接收指针
** 入口参数 	 ： type =ID,  转向所有  AT指令 字符串
				 	str_at- uart_buff   str_code -para_temp
**            	 	*str_at=要复制的内容/转AT指令
**            	 	*str_code 与*str_at合并的参数 //AT指令参数
** 出口参数 	：  1- 成功,  0- 失败
*********************************************************************************************************
*/
uchar  Send_AT_Command( uchar type,uchar *str_at ,uchar *str_code)      //发送ＡＴ指令
//拨号时号码放在phone.number
//其他用para_temp
{ 

    switch(type)
    {

	
     //----------------------------------------------------
      //--------------------------------------------------------
 
       case AT_CIPCLOSE:            //    AT+CIPCLOSE 关闭TCP 或UDP 连接
           strcpy(str_at,"ATE0V0+CIPCLOSE");
           break;
         case AT_CIPSHUT:            // AT+CIPSHUT 关闭移动场景
              strcpy(str_at,"AT+CIPSHUT");
           break;
       case AT_CLPORT:            // AT+CLPORT 设置本地端口
           strcpy(str_at,"AT+CLPORT=");
           strcat(str_at,str_code);
          break;
      case AT_CSTT:       //AT+CSTT 启动任务并设置APN、USER ID、PASSWORD
             strcpy(str_at,"AT+CSTT="); 
           strcat(str_at,str_code);
           break;

       case AT_CIICR:   // AT+CIICR 激活移动场景
             strcpy(str_at,"AT+CIICR");
           break;
        case AT_CIFSR:  //AT+CIFSR 获得本地IP 地址
           strcpy(str_at,"AT_CIFSR");
           break;
       case AT_CDNSGIP: //AT+CDNSGIP 域名解析
          strcpy(str_at,"AT+CDNSGIP=");
           strcat(str_at,str_code);
           break;
       case AT_CDNSCFG:   //AT+CDNSCFG 配置DNS
             // strcpy(str_at,"AT+CDNSCFG=\"202.105.21.217\"");
           //  strcpy(str_at,"AT+CDNSCFG=\"222.73.49.102\"");
             strcpy(str_at,"AT+CDNSCFG=");
             strcat(str_at,str_code);
             break;

        case AT_CDNSORIP:  //AT+CDNSORIP 设置连接SERVER 端为IP 地址还是域名 1 表示是域名
             strcpy(str_at,"AT+CDNSORIP=");
             strcat(str_at,str_code);
             break;
        case AT_CIPHEAD:      //AT+CIPHEAD 设置接收数据的IP 头
             strcpy(str_at,"AT+CIPHEAD=1;+CIPFLP=1;+CIPATS=1,50");
             
             break;
        case AT_CIPATS:        //AT+CIPATS 设置自动发送时间
            strcpy(str_at,"AT+CIPATS=1,50");
            
             break;
        case  AT_CIPSEND :    //  GPRS 发送
            strcpy(str_at,"AT+CIPSEND");
            break;
       case AT_CIPSERVER:  //AT+CIPSERVER 配置为服务器
           strcpy(str_at,"AT+CIPSERVER");
            break;
       case AT_CIPCSGP:  //  AT+CIPCSGP 设置为CSD 或GPRS 连接
             strcpy(str_at,"AT+CIPCSGP=");
             strcat(str_at,str_code);
             break;
      case AT_CIPCCON: // AT+CIPCCON 设置当前连接 返回1 表示当前TCP 连接的发
             strcpy(str_at,"AT+CIPCCON=");
             strcat(str_at,str_code);   //AT+CIPSHUT
             break;

       case AT_CIPSTART: //  //AT+CIPSTART   连接 GPRS TCp OR UDP
             strcpy(str_at,"AT+CIPSTART=");//AT+CIPSTART="TCP","llx1.xicp.net","8080"
             strcat(str_at,str_code);
             break;
      case AT_CIPDPDP:    //AT+CIPDPDP 设置网络状态检测的时间间隔
            strcpy(str_at,"AT+CIPDPDP=0");

             break; 
         case AT_CIPSTATUS:
            strcpy(str_at,"AT+CIPSTATUS");

             break;
         //----------------------------------------------------------

     //-----------------------------------------------
    
      default: 
      	return(FALSE);
    }
    ES=1;
    strcat(str_at,"\x0d\x00");//在命令后加入CR
    RI=0;					//清除接收标志
    TI=0;
   //------------------------------
  Print_Str(str_at);   
 
    return(TRUE); 

}


/*
*********************************************************************************************************
** 函数名称 	 GPRS_SEND()
** 函数功能		  TCP/UDP数据发送
** 全局变量或数组:   receive_count 接收指针
** 入口参数 	 ：  AT指令 字符串
				 	str_at- uart_buff   str_code -para_temp
**            	 	*str_at=要复制的内容/转AT指令
**            	 	*str_code  发送的内容
** 出口参数 	： 1-成功      0-失败
*********************************************************************************************************
*/
/**************************************************************/
//----------------------------------
//------GPRS发送---------------
void GPRS_SEND(char* ptr1_at)
{ 
 Send_AT_Command(AT_CIPSEND,ptr1_at,0);      //发送GPRS数据
 }

//-----------------

/*
*********************************************************************************************************
** 函数名称 	 GPRS_SET_UP()
** 函数功能		GPRS中的 TCP 协议连接   这里用的是域名方式连接
** 全局变量或数组:   receive_count 接收指针
** 入口参数 	 ：  AT指令 字符串
				 	str_at- uart_buff   str_code -para_temp
**            	 	*str_at=要复制的内容/转AT指令
**            	 	*str_code AT指令的参数或其它参数
** 出口参数 	：
*********************************************************************************************************
*/
void  GPRS_SET_UP(void)
{ 

   Send_AT_Command(AT_CDNSORIP,GPRSBuff,"1"); //IP方式
      ReadBsbuffStrstr(GPRSBuff,AT_Code); 
    Send_AT_Command(AT_CLPORT,GPRSBuff,"\"TCP\",3030");//
	 ReadBsbuffStrstr(GPRSBuff,AT_Code);  
  Send_AT_Command(AT_CIPSTART,GPRSBuff,"\"TCP\",\"122.234.243.143\",\"1001\"\x00");//
  if(ReadBsbuffStrstr(GPRSBuff,"8")!=FALSE) GPRSOK=0;//连接成功
  else GPRSOK=1;  //连接失败

}


//////////////////////////////////////////////////
void GPRS_Dispose(void) 
{
   uchar j=0,i=0;
  if(RS_Sets_Cont>0) 
   { 
     MygetSets(GPRSBuff);
		i=strsearch("+IPD",GPRSBuff,sizeof(GPRSBuff)); //IP头
            //+IPD5:LED3N  +IPD10:12345LED3N
            if(i)
            { 
             //-------报文长度,  这里跳过--------------
              i=i+3;
              i++;
              if(GPRSBuff[i]==':')// :后面是内容
              i++;
              else i=i+2;
              j=i;
              //-下面GPRS数据处理区------------
           //---------LED2N LED3N LED4N LED5N LED6N LED7N--------------------------------
 		   //         LED2F LED3F LED4F LED5F LED6F LED7F
 		 if(GPRSBuff[i]=='L'&&GPRSBuff[i+1]=='E'&&GPRSBuff[i+2]=='D'&&GPRSBuff[i+3]=='0'&&GPRSBuff[i+4]=='O'&&GPRSBuff[i+5]=='N')
           {            
				LED0=0;
                 //GPRS_SEND(ptr1_at,ptr1_code);
           }
		  else if(GPRSBuff[i]=='L'&&GPRSBuff[i+1]=='E'&&GPRSBuff[i+2]=='D'&&GPRSBuff[i+3]=='0'&&GPRSBuff[i+4]=='O'&&GPRSBuff[i+5]=='F'&&GPRSBuff[i+6]=='F')
           {            
				LED0=1;
                 //GPRS_SEND(ptr1_at,ptr1_code);
           } 
          

		 }
	}
 
     /*****************************************/
     

 }
/****************************************/ 
/******************************************/
void main(void)
{
 
 
   
	while(1)
	{

 
 	//------------以下是短信处理-------------------------------------
 			// 硬件中断处理 连GPRS 服务器
	
 GPRS_SET_UP();

      
      }
}
///////////////////////////////////////////////////////
/*********************************************************** 
函数名称：main
函数功能：主函数
***********************************************************/ 
 


//串口接收中断函数 
void serial () interrupt 4 using 3 
{ 
   if(RI==1)
  {
    RI=0;
    RsBuf[RsPoint++]=SBUF;
    if (RsPoint>=sizeof(RsBuf))
    {
      RsPoint=0; 
    }
 if((RsBuf[RsPoint-2]==0x0d)
     &&(RsBuf[RsPoint-1]==0x0a) )
   		    {
   		      RS_Sets_Cont++;      
   		    }   
  }
} 


/*
*********************************************************************************************************
** 函数名称 	Int_Timer0()
** 函数功能		定时器中断位,是整个程序的心跳,如果停止,程序会死机
** 全局变量或数组:

** 入口参数 	 ：
** 出口参数 	：
*********************************************************************************************************
*/

void  Int_Timer0(void) interrupt 1 using 3
{
     TH0 = 0xB8;
	 TL0 = 0x00; //20ms的时钟基准

     if(timercount>=1) timercount--; 
      
    
}
