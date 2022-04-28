//==**********************************************************
//--软件功能, 读一条英文短信，但不回复，  START  END  两条，可以控制继电器
//---   可使用单片机  要求 16K ROM 256 RAM 及以上 8位标准单片机   STC SST SM W78E单片机
//--   TC35 SIM300 GTM900 EM310   波特率 9600
//100%不支持  AT89S51 W78E51及其它RAM为 128B的单片机
//============================================================
#include <cheshi.h>
//#include <lcd.h>
uchar data int_case;	//中断标志
#define MAX_TM   100         // AT指收发数组最大缓冲
#define MAX_T  35
uchar idata uart_buff[MAX_TM]; //通信程序中的缓冲
uchar idata  receive_count;    //发送与接收 数组指针

uchar data para_temp[MAX_T];     //多用临时用
uchar data read_tmp;             ///读电话本号参数
uchar data sms_num_tmp;			///读短信号码参数
uchar data sms_tmp;			///短信处理参数代码
uchar idata TEL_temp[15];   //存对方号码，
#define IO_OUT			P3_6


//－－－定时用－－－－－－－－－－－
uint  data timercount;		//50MS定时器,
uchar data timer_1S_cnt;
uchar data timer_10S_cnt;
uchar data timer_S_cnt;
//--------------------
uchar data ring_cnt;    //振铃用
 bit ring_bit  ;  // 电话呼入　2





  /*
*********************************************************************************************************
** 函数名称 dmsec()
** 函数功能 ：延时 NS
** 入口参数 ：
** 出口参数 :
*********************************************************************************************************
*/
//---延时S------------------------------
void dmsec (unsigned int count)
{
	unsigned int i;
	while (count)
	{
		#if   CPU_TYPE2==W78E58
           i =115;
        #elif  CPU_TYPE2==STC89E58
           i =300;
        #endif

		while (i>0) i--;
		count--;
    }
}


//------------------------
//----------------------
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

   timer_1S_cnt++;   timercount++;
   if(timer_1S_cnt==50)    //20×50=1000MS  =1S
    { timer_10S_cnt++; timer_1S_cnt=0;
      timer_S_cnt++;
 	  LED_S0=~LED_S0;

    }                              //１秒＝１０００毫秒
    if(timer_10S_cnt==60)   //60S
    {
	   timer_10S_cnt=0;

	}

//---------------------------------------------------------------------------------

}

/*
*********************************************************************************************************
** 函数名称 	Int_Uart()
** 函数功能		 TC35 的命令列表,与中断服务程序
** 全局变量或数组:   receive_count 接收指针      uart_buff
                      ring_bit  ring_cnt
** 入口参数 	 ：
** 出口参数 	： 1-成功      0-失败
*********************************************************************************************************
*/

void  Int_Uart(void) interrupt 4 using 3  //串口的中断程序
{

	if(RI) //命令方式
    {

        	RI=0;

         if((receive_count<MAX_TM-1))//没有发送时才能进行接收
        	{	//所有的命令返回都是处于 0x0A [Result] 0x0D 之间,其他命令,不会出现这种情况
                uart_buff[receive_count++]=SBUF;
               //0D 0A 32 0D 0A
                if((receive_count==5)&&(uart_buff[0]==0x0d&&uart_buff[1]==0x0A))
                {
                   if(uart_buff[2]=='2')    //电话呼入
       			   {    ring_bit=1;  ring_cnt++;
       			       receive_count=0;
       			   }
                }
        	}
        	 else
        	 {
       			 return;
       		 }


      //------------------------------------

	}//End of if(RI)
}



  /*
*********************************************************************************************************
** 函数名称 Initialize_Model()
** 函数功能 ： 模块初始化
** 入口参数 ：ptr1_at --uart_buff AT指令发送用 ptr1_code -para_temp 参数内容
**             全局 变最    ptr1_at, uart_buff, receive_count
** 出口参数 :
*********************************************************************************************************
*/

void Initialize_Model(char* ptr1_at,char* ptr1_code)    //初始化　PIN检测
{
	uchar i ,j;
        P0=1;
	 BELL=0;  for(i=0;i<=100;i++) dmsec(1);  BELL=1;
   //  GPS_READ(ptr1_at,ptr1_code);

     ptr1_code[0]=0;
	for(j=0;j<15;j++)
	{   //
		Send_AT_Command(AT_E,ptr1_at);
		Send_AT_Command(AT_INIT,ptr1_at);
		if(ptr1_at[1]==0x0d&&ptr1_at[0]==0x30)
		{
		   P0_3=1;
		   break;
		}
        else if(ptr1_at[receive_count-1]==0x0d&&ptr1_at[receive_count-2]=='0')
         {  break;}

		if(i==1)   //开机
		{   IO_IGT=0;
		    for(i=0;i<=200;i++) dmsec(30);
		    IO_IGT=1;
		    timercount=0; while(timercount<60);
		    timercount=0; while(timercount<60);
		}


   	   timercount=0; while(timercount<60);
   	    timercount=0; while(timercount<60);

    }






 	Send_AT_Command(AT_IPR,ptr1_at);//波特率  AT+IPR=9600
    if(ptr1_at[1]==0x0d&&ptr1_at[0]==0x30)
		{
		   P0_4=1;

		}
  	Send_AT_Command(AT_CGMM,ptr1_at);  //模块型号 AT+CGMM

     BELL=0;  for(i=0;i<=100;i++) dmsec(1);  BELL=1;

}


  /*
*********************************************************************************************************
** 函数名称 Sys_Init()
** 函数功能 ： 单片机初始化
** 入口参数 ：
**             全局 变最  timer_1S_cnt  receive_count
** 出口参数 :
*********************************************************************************************************
*/

void  Sys_Init(void)//话机启动的初始化程序,包含键盘与定时器,串口等
{
			//TH0=(65536-46080)/256;//重新装入定时25mS的初值到TH0,TL0     56320
  			//TL0=(65536-46080)%256;
			//65536-(11.0592/12)*20MS*1000=To    18432     47104
			//1ms定时
			//11.0592/12 = 0.9216 M (记数频率)
			//1ms记数 921.6次
			//16位 2^16 - x = 922 x=64614 FC66
			//定时器里不断放fc66，不用怀疑51定时器得到的 1ms的精确性吗

	TH0 = 0xB8;
	TL0 = 0x00; //20ms的时钟基准

  //串口中断
    TI=0; RI=0;
    SCON=0x50;        		//选用方式1
    TMOD=0x20;
    TMOD = (TMOD & 0xf0) | 1;//MODE 1
   // TH1=0xe8;              //波特率为9600
   // TL1=0xe8;
   TH1=0xfd;              //波特率为9600
   TL1=0xfd;
    PCON=0x00;            //倍频0x80
    IE = 0xb0;  	// EA XX ET2 ES ET0 EX0 ET1 EX1
    TR1=1;  		//enable TIMER1

    ES=1;  //开串口开断
    TR0=1; //串口接收中断
    ET0=1;  //T0 中断
    ET1=0;  // T1 中断
    EA=1;
    P0=0xFF;
    P1=0xFF;
    P2=0xFF;
    P3=0xFF;
   // P4=0xff;
    timer_1S_cnt=0;receive_count=0;
    IO_IGT=1;



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
uchar  Send_AT_Command( uchar type,uchar *str_at)      //发送ＡＴ指令
//拨号时号码放在phone.number
//其他用para_temp
{
	uint   i;

    switch(type)
    {

      case AT_CPIN:		//检查当前是否要输入PIN码
        strcpy(str_at,"AT+CPIN?");
        break;
     //  AT_E
       case AT_E:		//ATE0 功能关回显  ATE1 开回显
        strcpy(str_at,"ATE0");
        break;
      case AT_INIT:		//ATV0  回复方式 30 0D  ATV1 回复方式  OK
        strcpy(str_at,"ATV0");
        break;
        //0D 0A 54 43 33 35 0D 0A
      case   AT_CGMM:   //模块版型号
        strcpy(str_at,"AT+CGMM");
        break;


     //----------------------------------------------------

         //----------------------------------------------------------

     //-----------------------------------------------
      case AT_COMMAND:
      	break;
      default:
      	receive_count=0;
      	return(TRUE);
    }
    ES=1;
    strcat(str_at,"\x0d\x00");//在命令后加入CR
    RI=0;					//清除接收标志
    TI=0;
   //------------------------------
    for(i=0;i<MAX_TM;i++)
	{	if(str_at[i]==0) break;
		SBUF=str_at[i];
		while(!TI);
		TI=0;
	}
   //---------------------
   receive_count=0;
   str_at[0]=0; str_at[1]=0;str_at[2]=0;
   i=0; timer_1S_cnt=0;
   do{
       dmsec(3);
       if(receive_count>1)
       {   //	dmsec(50);
       	    timercount=0; while(timercount<30);
            break;
       }

    }while(timer_1S_cnt<3);

    return(TRUE);



}


/******************************************/
void main(void)
{

	 Sys_Init();

	Initialize_Model(uart_buff,para_temp);//进行初始化


	receive_count=0;


	while(1)
	{
		//-----------查询1到20条中的短信,只要有短信就处理------------------------------------------
           	Send_AT_Command(AT_CGMM,uart_buff);  //模块型号 AT+CGMM
     if(uart_buff[receive_count-1]==0x0d&&uart_buff[receive_count-2]==0x30)
		{
		   P0_6=~P0_6;

		}
            timercount=0; while(timercount<60);
   	        timercount=0; while(timercount<60);


	}


}





























































