
#include <cheshi.h>
uchar data int_case;	//中断标志
#define MAX_TM   185         // AT指收发数组最大缓冲
#define MAX_T  60
uchar idata uart_buff[MAX_TM]; //通信程序中的缓冲
char idata  receive_count;    //发送  数组指针
 
#if  CPU_TYPE2==STC89E58
uchar idata  pc_bit  ;  //串口2接收中断
uchar xdata pc_buff[MAX_TM];
char idata  pc_count;
  #endif


uchar xdata para_temp[MAX_T];     //多用临时用
uchar data read_tmp;             ///读电话本号参数
uchar data sms_num_tmp;			///读短信号码参数

uchar data sms_tmp;			///短信处理参数代码
uchar xdata TEL_temp[20];   //存对方号码，
uchar xdata GPRS_TMP[32];     //多用临时用
//#define IO_OUT			P3_6
//#define GPS_ON			P1_1 //GPS开关
uchar data system_server ;    //系统状态

//－－－定时用－－－－－－－－－－－
uint  data timercount;		//50MS定时器,
uchar data timer_1S_cnt;
uchar data timer_10S_cnt;
uchar data timer_S_cnt;




//--------------------
uchar xdata LED_P0;
uchar data ring_cnt;    //振铃用
 bit ring_bit  ;  // 电话呼入　2
//------------------------------
uchar xdata PT2272_BUF;//存储用
bit PT2272_BIT;   //315M-是否连结到设备上
uchar bdata PT2272_TMP;  //无线控制
sbit PT2272_D0=   PT2272_TMP^0;
sbit PT2272_D1=   PT2272_TMP^1;
sbit PT2272_D2=   PT2272_TMP^2;
sbit PT2272_D3=   PT2272_TMP^3;
sbit PT2272_VT=   PT2272_TMP^4;
//-----------------------------------
//位变量




// 中断　IO 模拟脚
bit io_p00_bit;
bit io_p01_bit;
bit io_p02_bit;
bit io_p03_bit;
bit io_p04_bit;
bit io_p05_bit;
bit io_p06_bit;
bit io_p07_bit;
//　ＩＯ设置开启与关闭
bit io_p00_on;
bit io_p01_on;
bit io_p02_on;
bit io_p03_on;
bit io_p04_on;
bit io_p05_on;
bit io_p06_on;
bit io_p07_on;
//1MS
/*void Wait_ms
{
   unsigned int i;
	while (count)
	{
		i = 115;
		while (i>0) i--;
		count--;
    }

}   */
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

#if   CPU_TYPE2==STC89E58
//--------------------------
/*
*********************************************************************************************************
** 函数名称 	send_UART_two()
** 函数功能		串口2发送字符
** 全局变量或数组:

** 入口参数 	 ：
** 出口参数 	：
*********************************************************************************************************
*/
void send_UART_two(char* ptr1_at)
{
//sfr  SCON    = 0x98;
//SM0,SM1,SM2,REN,TB8,RB8,TI,RI

//sfr  S2CON    = 0x9A;
//S2SM0,S2SM1,S2SM2,S2REN,S2TB8,S2RB8,S2TI,S2RI
//sfr  S2BUF    = 0x9B;
//sfr  IE2    = 0xAF;
//X,X,X,X,X,X,ESPI,ES2

	unsigned char temp = 0,j=0;

//    ES     =   0;  //关串口1中断
	IE2	=	0x00;	//关串口2中断,es2=0
 do{
//    TI     =   0;  //清零串口1发送完成中断请求标志
   if(ptr1_at[j]==0) break;
    S2CON	=	S2CON & 0xFD; //B'11111101,清零串口2发送完成中断请求标志
//    SBUF   =   i;
    S2BUF   =   ptr1_at[j];
//    while(TI ==0); //等待发送完成
    do
	{
		temp = S2CON;
		temp = temp & 0x02;
	}while(temp==0);

  }while(ptr1_at[j++]!=0);
//	TI     =   0;  //清零串口发送完成中断请求标志
    S2CON	=	S2CON & 0xFD; //B'11111101,清零串口2发送完成中断请求标志
//    ES     =   1;  //允许串口1中断
//	ES2	=	1
	IE2	=	0x01;	//允许串口2中断,ES2=1
}


/*
*********************************************************************************************************
** 函数名称 	send_com_two()
** 函数功能		串口2发送字符
** 全局变量或数组:

** 入口参数 	 ：
** 出口参数 	：
*********************************************************************************************************
*/
void send_com_two(char* ptr1_at,uchar id)
{
//sfr  SCON    = 0x98;
//SM0,SM1,SM2,REN,TB8,RB8,TI,RI

//sfr  S2CON    = 0x9A;
//S2SM0,S2SM1,S2SM2,S2REN,S2TB8,S2RB8,S2TI,S2RI
//sfr  S2BUF    = 0x9B;
//sfr  IE2    = 0xAF;
//X,X,X,X,X,X,ESPI,ES2

	unsigned char temp = 0,j=0;

//    ES     =   0;  //关串口1中断
	IE2	=	0x00;	//关串口2中断,es2=0
 do{
//    TI     =   0;  //清零串口1发送完成中断请求标志
   if(j==id) break;
    S2CON	=	S2CON & 0xFD; //B'11111101,清零串口2发送完成中断请求标志
//    SBUF   =   i;
    S2BUF   =   ptr1_at[j];
//    while(TI ==0); //等待发送完成
    do
	{
		temp = S2CON;
		temp = temp & 0x02;
	}while(temp==0);

  }while(j++!=id);
//	TI     =   0;  //清零串口发送完成中断请求标志
    S2CON	=	S2CON & 0xFD; //B'11111101,清零串口2发送完成中断请求标志
//    ES     =   1;  //允许串口1中断
//	ES2	=	1
	IE2	=	0x01;	//允许串口2中断,ES2=1
}

 /*
*********************************************************************************************************
** 函数名称 serial_port_two_initial()
** 函数功能 ： STC12C5A32S2 双串口功能
** 入口参数 ：
**             全局 变最
** 出口参数 :
*********************************************************************************************************
*/
void serial_port_two_initial()
{
/*
12T  reload=256-INT(fosd/reload/32/12+0.5)
1T   reload=256-INT(fosd/reload/32+0.5)
//	reload 产生的波特率

//baud=fosd/(256-reload)/32/12      12T
//baud=fosd/(256-reload)/32          1T
//  fosd=22.1184MHZ , baud=57600 (12T)
    reload=256-INT(22118400/57600/32/12+0.5)
        =256-INT(1.5)
        =256-1
        =255
   baud= 22118400/(256-255)/32/12
       =57600
       本机采用11.22118400  ,波特率 38400
      reload=256-INT(11059200/38400/32+0.5)
            =256- 9
            =247

 */

//sfr  SCON    = 0x98;
//SM0,SM1,SM2,REN,TB8,RB8,TI,RI

//sfr  S2CON    = 0x9A;
//S2SM0,S2SM1,S2SM2,S2REN,S2TB8,S2RB8,S2TI,S2RI
//sfr  S2BUF    = 0x9B;
//sfr  IE2    = 0xAF;
//X,X,X,X,X,X,ESPI,ES2

	S2CON    =   0x50;   //0101,0000 8位可变波特率，无奇偶校验位,允许接收
	BRT	=	247;
//AUXR BRTR = 1 充许独立波特率发生器工作
//     S1BRS = 1   独立波特率发生器作为串口波特率发生器,T1可用
//     EXTRAM = 0  开启外部RAM
//   BRTR = 1  S1BRS = 1, EXTRAM = 0 ENABLE EXTRAM
	//AUXR	=	0x11; // T0x12,T1x12,UART_M0x6,BRTR,S2SMOD,BRTx12,EXTRAM,S1BRS
      AUXR	=	0x14;
//    ES      =   1;    //允许串口1中断
//	ES2	=	1
	IE2	=	0x01;	//允许串口2中断,ES2=1
  //  EA      =   1;    //开总中断
}


void Interrupt_Receive_2(void) interrupt 8
{
//sfr  SCON    = 0x98;
//SM0,SM1,SM2,REN,TB8,RB8,TI,RI

//sfr  S2CON    = 0x9A;
//S2SM0,S2SM1,S2SM2,S2REN,S2TB8,S2RB8,S2TI,S2RI
//sfr  S2BUF    = 0x9B;
//sfr  IE2    = 0xAF;
//X,X,X,X,X,X,ESPI,ES2

	unsigned char   k   =   0;
	k = S2CON ;
	k	= k & 0x01;
	//if(S2RI==1)
    if(k==1)
    {
        //RI  =   0;
		S2CON = S2CON & 0xFE; //1111,1110
       // k   =   S2BUF;
       if((pc_count<MAX_TM-1))//没有发送时才能进行接收
        	{   pc_buff[pc_count++]=S2BUF;
        		pc_buff[pc_count]=0;
        		pc_bit=1;    //接收到数据,进入中断请求

        	}
       else
        {
       	   return;
        }

    }
    else
    {
        //TI  =  0;
		S2CON = S2CON & 0xFD; //1111,1101
    }
}

#endif

//------------------------------------------

 //硬件处理
  /*
*********************************************************************************************************
** 函数名称 int_p02()
** 函数功能 ： 按键 处理
** 入口参数 ：ptr --at_buf AT指令发送用 ptr1_code  短信内容,ptr2,TEL_temp 电话号码,
**            ID -暂时参数   ,全局 变最 read_tmp  ptr1_at, uart_buff,0,TEL_temp receive_count
** 出口参数 :
*********************************************************************************************************
*/      //char* ptr1_at,char* ptr1_code, char* ptr_tel
void int_p02(char* ptr1_at,char* ptr1_code, char* ptr_tel) // 发短中文短信前设置。 P0.2灯闪，同时蜂鸣器长叫一声
{    uchar tmp_i;

     BELL=0;
     for(tmp_i=0;tmp_i<8;tmp_i++)
     {  dmsec(100);
     }
 	BELL=1; //蜂鸣器

     io_p00_on=1; //清中断


      //读电话本１
      read_tmp=1  ;
   	  tmp_i=PHONE_RD(ptr1_at,ptr1_code,read_tmp);
   	  ptr1_code[tmp_i++]=0;
   	  ptr1_code[tmp_i++]=0;

   	  tmp_i=0;   // 将读出的电话号码转存起来，做发送对方号码
   	  do{
   	       ptr_tel[tmp_i]=ptr1_code[tmp_i];
   	    }while( tmp_i++<=20);

       //回中文短信
      send_sms(ptr1_at,ptr1_code,ptr_tel,smss_pud_2); //调用发短信

}

//硬件处理
 /*
*********************************************************************************************************
** 函数名称 int_p03()
** 函数功能 ： 按键 处理
** 入口参数 ：ptr --at_buf AT指令发送用 ptr1_code  短信内容,ptr2,TEL_temp 电话号码,
**            ID -暂时参数   ,全局 变最 read_tmp  ptr1_at, uart_buff,0,TEL_temp receive_count
** 出口参数 :
*********************************************************************************************************
*/
//发短中文短信前设置。 P0.2灯闪，同时蜂鸣器长叫一声
void int_p03 (char* ptr1_at,char* ptr1_code, char* ptr_tel ) //
{      uchar i;
     BELL=0;
     for(i=0;i<8;i++)
     {  dmsec(100);
     }
     BELL=1;  //蜂鸣器
      io_p02_on=1;

     //----------------------------
      //读电话本１ para_temp[0]='1'
       read_tmp=1;
   	  i=PHONE_RD(ptr1_at,ptr1_code,read_tmp);
   	  ptr1_code[i++]=0;

   	 //----------------------
   	 //__________________________________
   	  i=0;   // 将读出的电话号码转存起来，做发送对方号码
   	  do{
   	       ptr_tel[i]=ptr1_code[i];
   	    }while( i++<=20);
   	  //------------------------------------
       //英文短信回
      send_sms(ptr1_at,ptr1_code,ptr_tel,smss_text_1);       //  //调用发短信


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
    if(timer_10S_cnt==10)   //10S
    {
	   timer_10S_cnt=0;

	}

  //---------硬件中断处理---------------------------------------------------------------------
   //-------P2.7-D0,P2.6-D1,P2.5-D2,P2.4-D3,    P2.3-VT-315M有信号中断，----------------------------
   //如果有PT2272接入 P2接入口会为低，P2.3-P2.7=0;
    P2=0xff;
    PT2272_BUF= P2&0x0F;     //加VT就是0xf1
    if(PT2272_BUF==0)  //状态检测
    {    PT2272_BIT=1; //可以进行中断
    }

    else if(((PT2272_BUF>0)&&((PT2272_BUF&0x0f)!=0x0f))&&PT2272_BIT==1) //这种方法只用于PT2272-M4的芯片
    {   PT2272_TMP= PT2272_BUF;
    	PT2272_BIT=0;
    }

    //---------------------------

    IO_P25=1;        //外部中断　Ｐ２5口
	if((io_p00_bit!=IO_P25))
	{
		if(IO_P25==0)
		{ io_p00_on=0;
		}

		io_p00_bit=IO_P25;
	}

    IO_P24=1;        //外部中断　Ｐ24口
	if((io_p02_bit!=IO_P24))
	{
		if(IO_P24==0)
		{ io_p02_on=0;
		}

		io_p02_bit=IO_P24;
	}

	IO_P35=1;        //外部中断　Ｐ35口
	if((io_p03_bit!=IO_P35))
	{
		if(IO_P35==0)
		{ io_p03_on=0;
		}

		io_p03_bit=IO_P35;
	}




 //------------------------------------------------------------




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
          if(GPS_ON==1)
          {
        	if((receive_count<MAX_TM-1))//没有发送时才能进行接收
        	{	//所有的命令返回都是处于 0x0A [Result] 0x0D 之间,其他命令,不会出现这种情况
                uart_buff[receive_count++]=SBUF;
                uart_buff[receive_count]=0;
               //0D 0A 32 0D 0A
                if((receive_count==5)&&(uart_buff[0]==0x0d&&uart_buff[1]==0x0A))
                {
                   if(uart_buff[2]=='2')    //电话呼入
       			   {    ring_bit=1;  ring_cnt++;
       			       receive_count=0;
       			   }
                }
                else   if((receive_count>=5&&receive_count<=10)&&(uart_buff[receive_count-2]==0x0d&&uart_buff[receive_count-1]==0x0A))
                {
                   if(uart_buff[receive_count-3]=='2')    //电话呼入
       			   {    ring_bit=1;  ring_cnt++;
       			       receive_count=0;
       			   }
                }
        	}
        	 else
        	 {
       			 return;
       		 }
       	 }
      //--------GPS专用-------------------------------
       else
       {
            if((receive_count<MAX_TM-3))//没有发送时才能进行接收
        	{	//所有的命令返回都是处于 0x0A [Result] 0x0D 之间,其他命令,不会出现这种情况
                uart_buff[receive_count++]=SBUF;
        	}
        	 else
        	 {  // receive_count=0;
       			 return;
       		 }
            if((uart_buff[receive_count-2]==0x0d)
   			   &&(uart_buff[receive_count-1]==0x0a) )
   		    {
   		           return;

   		    }

       }
      //------------------------------------

	}//End of if(RI)
}

/*********************************************************************
 *                  C51中字符串函数的扩充                            *
 ** 函数名称    : strsearch ()
 ** 函数功能    : 在指定的数组里连续找到相同的内容
 ** 入口参数   ： ptr2=要找的内容, ptr1 当前数组
 **** 出口参数 ： 0-没有 找到   >1 查找到
 *********************************************************************/


uchar strsearch(uchar *ptr2,uchar *ptr1_at)//查字符串*ptr2在*ptr1中的位置
//本函数是用来检查字符串*ptr2是否完全包含在*ptr1中
//返回:  0  没有找到
//       1-255 从第N个字符开始相同
{
//、	uchar max_length;
	uchar i,j,k;
	uchar flag;
	if(ptr2[0]==0) return(0);
	flag=0;
	for(i=0,j=0;i<MAX_TM-2;i++)
	{
        if(ptr1_at[i]==ptr2[j])
        {//第一个字符相同
        	for(k=i;k<MAX_TM-2;k++,j++)
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

/*
*********************************************************************************************************
** 函数名称 	 Read_Call_ID()
** 函数功能		 //   读来电号码
** 全局变量或数组:   receive_count 接收指针
** 入口参数 	 ：  AT指令 字符串str_code -para_temp  	str_at- uart_buff
             	 	*str_at=要复制的内容/转AT指令
            	 	*ptr_tel   电话号码

** 出口参数 	： 1-成功      0-失败
*********************************************************************************************************
*/
uchar Read_Call_ID(char* ptr1_at,char* ptr1_tel)//读出来电显示的号码
//正确  TRUE
//错误  FALSE
//号码  string_temp[0..16],最多显示前16位
//呼叫方式 stringt_temp[20]
{
	uchar idata i,j,r;

	Send_AT_Command(CALL_ID,ptr1_at,0);//+CLCC:1,1,4,0,0,"02138950224",129
	//	语音呼入
    timercount=0; while(timercount<50);
	   //读来电显示出错时设为1
	if(receive_count<15) return(FALSE);
	ptr1_at[receive_count]=0;
      receive_count=0;
	j=0;
//	i=strsearch(uart_buff,"CLCC");//查第一个CLCC
    i=strsearch("CLCC",ptr1_at);//查第一个CLCC
	if(i==0) return(FALSE);
	j=j+i;
	for(r=0;r<9;r++)
	{
	if(ptr1_at[j]==',')   break;
	else  j=j+1;
   }
	if(r>=9) return(FALSE);
	j=j+1;
	for(r=0;r<4;r++)
	{
		if(ptr1_at[j]==',')    break;
		else  j=j+1;
    }
	if(r>=4) return(FALSE);
	j=j+1;
	//检查是否为来话呼叫
	if(ptr1_at[j]!='4') return(FALSE);
	for(r=0;r<4;r++)
	{
	if(ptr1_at[j]==',')    break;
	else  j=j+1;
    }
	if(r>=4) return(FALSE);
	j=j+1;
//	string_temp[20]=uart_buff[j];//读出呼叫类型
	//只要能读出呼叫类型就认为正确
   //	i=strsearch("\x22",uart_buff);//查第一个"号
   	for(r=0;r<6;r++)
	{
	if(ptr1_at[j]=='\x22')   break;
	else  j=j+1;
    }

	if(r>=6)
		return(FALSE);
   	j=j+1;
    i=j;
   //	i=strsearch( "\x22",uart_buff);//查第二个"号
   	for(r=0;r<20;r++)
	{
	if(ptr1_at[j]=='\x22')   break;
	else  j=j+1;
   }
   if(i<=2) return(FALSE);//没有来电号码

   //存号码  TEL_temp
   for(r=0;r<22;r++)
	{
       ptr1_tel[r]=ptr1_at[i++];
	   if(ptr1_at[i]=='\x22')
	   {      r++;
	   	 ptr1_tel[r]=0;
	   	 ptr1_at[i++]=0;

	   	 break;
	   }

	 }

    if(r>=22)   return(FALSE);//出错
      ptr1_at[i++]=0;
     if(r>8) //号码大于8位说明是常用 号码,不是服务号码
   	    return(TRUE);
   	 else   return(FALSE);

}

/*
*********************************************************************************************************
** 函数名称 	 ring_auto()
** 函数功能		 //   电话呼入 处理
** 全局变量或数组:   receive_count 接收指针   ring_bit  ring_cnt
** 入口参数 	 ：  AT指令 字符串str_code -para_temp  	str_at- uart_buff
             	 	*str_at=要复制的内容/转AT指令
                    str_code 内容
            	 	*ptr_tel   电话号码

** 出口参数 	：
*********************************************************************************************************
*/
// //char* ptr1_at,char* ptr1_code, char* ptr_tel
void ring_auto( char* ptr1_at,char* ptr1_code, char* ptr_tel)
{

     if(Read_Call_ID(ptr1_at,ptr_tel))    //读来电显示
        {
                    IO_OUT=~IO_OUT; //smss_text_tmp
                    Send_AT_Command(HOOKOFF,ptr1_at,0);
                     Send_AT_Command(HOOKOFF,ptr1_at,0);
                     if(IO_OUT==1)  //断开
                        strcpy(ptr1_code,"CLOSE\x1a");
                     else           //吸合
                         strcpy(ptr1_code,"OPEN\x1a");
      				  //  //调用发短信
      				 send_sms(ptr1_at,ptr1_code,ptr_tel,smss_text_tmp);
        }
        Send_AT_Command(HOOKOFF,ptr1_at,0);
        Send_AT_Command(HOOKOFF,ptr1_at,0);
       ring_bit=0; ring_cnt=0;
      return;
}
//----------------------------------------------
/*
*********************************************************************************************************
** 函数名称 	 : P2_INIT()
** 函数功能		 这里进行中断查询如果有中断马上处理-
** 全局变量或数组:   receive_count 接收指针
** 入口参数 	 ： type =ID,  转向所有  AT指令 字符串
				 	str_at- uart_buff   str_code -para_temp
**            	 	*str_at=要复制的内容/转AT指令
**            	 	*str_code  内容或参数
** 出口参数 	：  1- 成功,  0- 失败
*********************************************************************************************************
*/
//---------这里进行中断查询如果有中断马上处理----------------------
uchar P2_INIT()
{     uchar  tmp_j;

      if(!io_p00_on )
        { io_p00_on=1;
           //system_server=SYS_KEY_2;
           return(SYS_KEY_2);
         }
      else if(!io_p02_on)
        {   io_p02_on=1;
           // system_server=SYS_KEY_1;
            return(SYS_KEY_1);
        }
      if(!io_p03_on )
       {   io_p03_on=1;
     		 BELL=0;
     		 for(tmp_j=0;tmp_j<8;tmp_j++)
     		 {  dmsec(100);}
     		 BELL=1;  //蜂鸣器
         system_server=SYS_DIAL;
         return(0);
       }

     if(ring_bit)
     {    ring_bit=0;
          BELL=0;
     	 for(tmp_j=0;tmp_j<8;tmp_j++)   {  dmsec(100);}
     	 BELL=1;  //蜂鸣器
        // system_server=SYS_RING ;
         return(SYS_RING);
      }


     //---------------------------------------------------
      return(0);
}
//----------------------------------------------------------------
/*
*********************************************************************************************************
** 函数名称 	 LED_1()
** 函数功能		灯闪与 无线接键处理 uart_buff
** 入口参数
** 出口参数 	：
*********************************************************************************************************
*/
 //-----灯闪与 无线接键处理,--------------------------------------------
void LED_1()
{
	LED_P0++;
	if(LED_P0>=7) LED_P0=0;
   if((PT2272_TMP>0))      //513M中断后处理。这里只是让灯亮几秒钟
       {  P0=0;
       	  if(PT2272_D0==1)  //A键      D1灯亮
          { P0_7=1;
            timercount=0; while(timercount<215) watch_dog_clear;
            P0_7=0;PT2272_D0=0;
          }
          if(PT2272_D1==1) //B键       D6灯亮
          { P0_6=1;
            timercount=0; while(timercount<215) watch_dog_clear;
            P0_6=0; PT2272_D1=0;
          }
          if(PT2272_D2==1) ////C键    D8灯亮
          { P0_5=1;
            timercount=0; while(timercount<215) watch_dog_clear;
            P0_5=0; PT2272_D2=0;
          }
          if(PT2272_D3==1)  // //D键  D9灯亮
          { P0_4=1;
            timercount=0; while(timercount<215) watch_dog_clear;
            P0_4=0; PT2272_D3=0;
          }

       	}
       else
       {
        if(LED_P0<=7)     //流水灯
       	 {  timercount=0; while(timercount<10)  ;
       		P0=(0x80>>LED_P0);
       	 }
       	 else
       	 {P0=(0x80>>LED_P0-8);}
       }




}

/*
*********************************************************************************************************
** 函数名称 	 CMGD_GSM()
** 函数功能		  删除短信   uart_buff
** 全局变量或数组:   uart_buff   sms_num_tmp
** 入口参数 	 ：  AT指令 字符串
             	 	*str_at=要复制的内容/转AT指令
                    id    短信ID  供删除短信时用
** 出口参数 	：
*********************************************************************************************************
*/
//----------------------------------
//删除短信   uart_buff
void CMGD_GSM(char* ptr1_at,uchar id)
{
	 uchar  ptr1[3] ;
     	ptr1[0]=(id/10)+0x30;
     	ptr1[1]=(id%10)+0x30;
     	ptr1[2]=0;
	  Send_AT_Command(SMS_CMGD,ptr1_at,ptr1);    //删除其它没有用的短信
	  Send_AT_Command(SMS_CMGD,ptr1_at,ptr1);


}

/*
*********************************************************************************************************
** 函数名称 	 CPMS_SM()
** 函数功能		  功能 读短信报文 ,有短信就读短信 ,把有短信的ID找到,并读出内容
** 全局变量或数组:   receive_count 接收指针  uart_buff  sms_num_tmp
** 入口参数 	 ：  AT指令 字符串
             	 	*str_at=要复制的内容/转AT指令
                    sms_num_tmp     短信ID  供删除短信时用

** 出口参数 	： 1-成功 得到短信存储最大值      0-失败
*********************************************************************************************************
*/

uchar CPMS_SM(char* ptr1_at )
//----------------------
{   uchar i,j,r,num_tmp1;
      for(i=0;i<=100;i++) ptr1_at[i]=0;
      Send_AT_Command(AT_CPMS,ptr1_at,0);
      timercount=0; while(timercount<15);
      if(i=strsearch("+CPMS:",ptr1_at)) //"SM"  +CPMS: 0,20,0,45,0,45
			 { i=i+5;
			     //+CPMS: 0,20,0,45,0,45
			  if(strsearch1(i,"\x20\x30,",ptr1_at))  //没有短信
		         {     return(0);
		         }
			  else  if((ptr1_at[i]==' ')&&((ptr1_at[i+1]>'1')  //9以下短信处理
				    &&(ptr1_at[i+1]<='9'))&&(ptr1_at[i+2]==','))
		       {      //+CPMS: 1,20,1,20,1,20
		       	   i=i+1;
				   r=ptr1_at[i]-0x30;   //有几个短个
				   i++;//,
				   i++; //20
				   j=0;
				   j=ptr1_at[i]-0x30;
				   j=j*10; //高位
				   i++;
				   j=j+(ptr1_at[i]-0x30); //低位
				   i++;
				   if(ptr1_at[i]!=','&&ptr1_at[i+1]==',') //可以存100以上的
				   { j=j*10;i++;
				     j=j+(ptr1_at[i]-0x30);

				  }
                 //返回是可以存多少短信
				  //r=j+10;
			 }
			  else if((ptr1_at[i]==' ')&&((ptr1_at[i+1]>'0')  //10条短信以上处理
				    &&(ptr1_at[i+2]>='0'))&&(ptr1_at[i+3]==','))
		       {
		       	   i=i+1;
				   r=ptr1_at[i++]-0x30;
				   r=r*10;   //移到十位
				   r=r+ptr1_at[i++]-0x30;

				   i++; //20
				   j=0;
				   j=ptr1_at[i]-0x30;
				   j=j*10; //高位
				   i++;
				   j=j+(ptr1_at[i]-0x30); //低位
				   i++;
				   if(ptr1_at[i]!=','&&ptr1_at[i+1]==',') //可以存100以上的
				   { j=j*10;i++;
				     j=j+(ptr1_at[i]-0x30);

				  }
                  //返回是可以存多少短信
				  //r=j+10;

			    }

			}
           else   //出错处理
           {   if(ptr1_at[1]==0x0d&&ptr1_at[0]==0x34)
           		{
 				    return(0);
           	    }

           }

         r=j;
  //---------下面有短信时,进行短信查找 从1-100------------------------------------
         num_tmp1=1;
        do{
            if(r>=100)  //短信存储号不能超过100个
                r=1; //
            if(r==1)         //如果是一条发AT+CMGL=ALL或AT+CMGL=4
                {

                   Send_AT_Command(SMS_CMGL,ptr1_at,0);
				    timercount=0; while(timercount<20);
				            //     +CMGL: 1,1,,35
				            i=strsearch("+CMGL: ",ptr1_at) ;

				   if(i)  //查短信代码回复是否对
				    {
				    	  i=i+6;
				      if(ptr1_at[i+1]==',')
					  {
					    num_tmp1=ptr1_at[i]-0x30; //当前短信存储号 10以内
					  }
					  else        //当前短信存储号 10以上
					  {
					      num_tmp1=ptr1_at[i++]-0x30;
					      num_tmp1=num_tmp1*10;
						  num_tmp1=num_tmp1+ptr1_at[i++]-0x30;

					  }
				      sms_num_tmp=num_tmp1;
				      return(1);
				    }

                }
           		else
				{
					i= READ_TEL(ptr1_at,num_tmp1) ;
					if(i==1) //有短信跳出
				  	{    i=i+7;
				       sms_num_tmp=num_tmp1;
				       return(1);
				  	}
                   else if(i==4)
                   {
                      return(0);
                   }
				}

           		r--;
           		num_tmp1++; //
       }while(r!=0);
      //---------------------------------------------
      receive_count=0;
       return(0);


}


/*
*********************************************************************************************************
** 函数名称 	 read_sms()
** 函数功能		  进行SIM卡的电话本读
** 全局变量或数组:   receive_count 接收指针
** 入口参数 	 ：  AT指令 字符串str_code -para_temp  	str_at- uart_buff
             	 	*str_at=要复制的内容/转AT指令
                    str_code 短信内容
            	 	*ptr_tel   电话号码

** 出口参数 	： 1-成功      0-失败
*********************************************************************************************************
*/

//====================================================
uchar  read_sms(char* ptr1_at,char* ptr1_code,char* ptr_tel,char* ptr_gps)
{

	 uint  i;
     uchar idata  j,  PDU_TEXT,y;
     if(!CPMS_SM(ptr1_at))
     {     //没有 短信返回
     		return(0);
     }
      /******************************************/
      //   //短信中心号码字头  //如果不是,说明没有短信
      /****************************************************/
      if((i=strsearch("08916831",ptr1_at)))
		 {  i=i+17;  j=0;
		      //带+86的中国移动与联通手机
		    if((ptr1_at[i]>='0')&&(ptr1_at[i+1]>='0'))
			{    i=i+2;  //当前指针移动两位
			     //_______取出对方号码长度______________
			     j=ptr1_at[i++]-0x30;  //取高位
			     j=j*16;   //称入高位
			     if(ptr1_at[i]>=0x41)    //大于9的低位处理
				 {
				 	 j=j+(ptr1_at[i++]-0x37);
				 }
				 else    //9及以下处理
				   j=j+(ptr1_at[i++]-0x30);
			     //___________________________
			      //-接入码-- 9186  与A1 接入码字头
				  if(ptr1_at[i]=='9') //
				  {
				  	i=i+4;  //指针移动4位,去掉 9186
				  }
			      else
			        i=i+2;  // 指针移动2位,去掉 A1
				  //j=j-1;
				 //------------对方号码查找---------------
				 //--电话号码存入 TEL_temp数组
				   y=0;
			      do {
			      	   j--;j--;
                       ptr_tel[y++]=ptr1_at[i+1];
                       if(ptr1_at[i]=='F') //有补码
                       {  ptr_tel[y]=0;
                          i++; i++;
					      break;
                       }
                       ptr_tel[y++]=ptr1_at[i];
                       i=i+2;
                         if(y>20) break; //长度不能超过20
			      	}while(j!=0);
					ptr_tel[y]=0;
				 //---------------------------------
                      //i=i+1;
                      //是否是中文模式 0008   2100
					if((ptr1_at[i]>='0')&&(ptr1_at[i+1]>='0')&&(ptr1_at[i+2]>='0')
					&&(ptr1_at[i+3]=='8'))
					{
					  i=i+4; //移动4位,去掉本0008
                      i=i+14; //移动14位,去掉本日期及时间
					  i=i+2;
                       //----------内容复制-将内容放到para_temp里------
                       for(j=0;j<=MAX_T-3;j++)  //不能超过最大值
                       {
                             ptr1_code[j]=ptr1_at[i++];
						  if(ptr1_at[i]==0x0d)
						  {  ptr1_code[++j]=0; break;
						  }
                          PDU_TEXT=0;   //PDU
                          if(i>=MAX_TM)  break;
                       }
                      //---------------------
					}
			        // 英文模式
			         else
			        {    //AT_CMGF1
                         Send_AT_Command(AT_CMGF1,ptr1_at,0); //转成英文读
                         Send_AT_Command(AT_CMGF1,ptr1_at,0); //转成英文读
                         timercount=0; while(timercount<8)  ;
                         READ_TEL(ptr1_at,sms_num_tmp );  //再次以英文读刚才短信
                         //从短信代码中找到维一代码与内容间的字符
                         if((i=strsearch("\x22\x0d\x0a",ptr1_at)))
     						{     i=i+2;//指针移动两位,指向内容的第一个字符

     						      //----内容复制-将内容放到para_temp里------------
     						       for(j=0;j<=MAX_T-3;j++)
                     			   {
                          			  ptr1_code[j]=ptr1_at[i++];
                          			  //大小写转换
                                      toupper(ptr1_code[j]);
						  				if(ptr1_at[i]==0x0d)
						  			{ ptr1_code[++j]=0; break;
						  			}
                     			    if(i>=MAX_TM)  break;
                     			   }
                                 //---------------------
     						}
                           else   goto err_dll;
			             PDU_TEXT=1;//文本

			      }
			        //i++;

		     } //固定代码对不对
             else goto err_dll;



		} //08916831
		//------------------------
         else
            goto err_dll;
    //------------------------------------------
 /***************************************************/
 /******************************************/
   //中文内容分析
/*******************************************/
    if(PDU_TEXT==0)
    {     i=0;
    	  //5438 5408  吸合
    	if(strsearch1(i,"54385408",ptr1_code))
   			 {

                   timercount=0; while(timercount<200) watch_dog_clear;
                      //有些比较重要的东西你可以存到卡就，重新开机也没有问题
		     	 	IO_OUT=0;
		     		strcpy(ptr1_code,"SET OK\x1a");
		     		system_server=SYS_TEXT_1;
		     		return(1);

   			 }
        //  断开 65AD 5F00   PUD码
       else  if(strsearch1(i,"65AD5F0",ptr1_code))
   			 {

                   timercount=0; while(timercount<150) watch_dog_clear;
		     		 IO_OUT=1;
		     		 system_server=SYS_PDU_1;  return(1);

   			 }
            else goto err_dll;



    }
  /***********************************/
  /***********************************/
  //   英文短信内容处理
  /***********************************/
  //------------------------------------------------------------
   else   if(PDU_TEXT==1)//英文
     {    i=0;
	   if(strsearch1(i,"START",ptr1_code)) // 继电器吸合
	   {


                   timercount=0; while(timercount<100) watch_dog_clear;
                      //有些比较重要的东西你可以存到卡就，重新开机也没有问题
		     	 	IO_OUT=0; //继电器吸合
		     	    strcpy(ptr1_code,"SET OK\x1a");
		     		system_server=SYS_TEXT_1; //转向短信回复
		     		 return(1);

		}
		else if(strsearch1(i,"END",ptr1_code))   // 发中文
			 {


                   timercount=0; while(timercount<110) watch_dog_clear;
		     		IO_OUT=1;   // 继电器断开
		     		system_server=SYS_PDU_1;//转向短信回复
		     		 return(1);

		}

       //_____电话本存储 TEL:1,13818120592  TEL:9,13818120592   存的内容是TEL_temp里的内容,为对方号码
       else if(j=strsearch1(i,"TEL:",ptr1_code))   //TEL:1,13818120592
			 {               //向前移动四位
			 	 i=i+4;
			 	j=0;
			 	if((ptr1_code[i]>='1')&&(ptr1_code[i]<='9'))

			     	ptr1_code[j++]=ptr1_code[i++];
			 	else  goto err_dll;
			 	i++;
			 	ptr1_code[j++]=',';
			 	ptr1_code[j++]='"';
			 // for(i=0;i<=20;i++)
			 do
			  {     if(ptr1_code[i]==0x0d||ptr1_code[i]==0x00)
                        {break;}
			  	   else
			  	      ptr1_code[j++]=ptr1_code[i] ;
			  	      i++;
			   }  while(ptr1_code[i]!=0x0d);
			 ptr1_code[j++]='"';ptr1_code[j++]=0;

              Send_AT_Command(PHONE_WRITE,ptr1_at,ptr1_code); //存电放本

              system_server=SYS_PDU_1;
               return(1);

		   }
		//_____________________________________________
		   else if(i=strsearch1(i,"GPS",ptr1_code))
		 {
                          GPS_READ(ptr1_at,ptr1_code,ptr_gps,0);
                          strcat(ptr1_code,"\x1a");

                          system_server=SYS_TEXT_1; //转向短信回复
                          return(1);
              }
		//---------------------------------------------------
		   else goto err_dll;


		}

	//-------------------------------------------------------------------

			else {

err_dll:			//短信号码删除 AT+CMGD=01
					CMGD_GSM(ptr1_at,  sms_num_tmp );

					for(i=1;i<=MAX_TM-3;i++)
         			{    ptr1_at[i]=0; }

         		   	Send_AT_Command(AT_CMGF0,ptr1_at,0);  //中文方式 //ATE0V0^SSMSS=1+CNMI=2,1
				//	Send_AT_Command(AT_SSMSS,ptr1_at,0);
					// return;

				 }



             return(0);





}
/*********************************************************************************************************
** 函数名称 ：pdu_set()
** 函数功能 ：GSM模块发送短信
** 入口参数 ：ptr --at_buf AT指令发送用 ptr1_code  短信内容,ptr_tel,TEL_temp 电话号码,
**            ID -暂时参数  可以转向发短信方式
   全局 变最     receive_count
** 出口参数 :
*********************************************************************************************************
*/
uchar pdu_set(char* ptr1_at,char* ptr1_code,char* ptr_tel)
{     uchar idata ptr_tmp[3];
      uchar idata r,y;
	  uchar idata i=0,j=0;

     do{          //PDU内容一定是4位为一个中文 ,
         i++;
     }while(ptr1_code[i]!=0);
//0011000D91683118180295F20008A70A8F66670953719669FF01
            r=i;
            i=(i/2)+15;//PDU长度报文
            ptr_tmp[0]=(i/10)+0x30;
     	 	ptr_tmp[1]=(i%10)+0x30;
     		ptr_tmp[2]=0;
			//短信号码长度，如果每加一个中文，长度加２
            Send_AT_Command(SMS_CMGS,ptr1_at,ptr_tmp);
            dmsec(50);
 			 y=0;
           	 strcpy(ptr1_at,"0011000D9168");
             y=12;
			 j=0;  //发送对方号码
				 //对方手机号码 进行 PDU转换
			   // 将TEL_temp里的 13818120592转换3118180295F2后放到 para_temp里
					do
					{
				      ptr1_at[y++]=ptr_tel[j+1];
				      ptr1_at[y++]=ptr_tel[j];
				      j=j+2;
				      if(ptr_tel[j+1]==0x0d||ptr_tel[j+1]==0)
				      { ptr1_at[y++]='F';
				      	ptr1_at[y++]=ptr_tel[j];
				      	break;
				      }
				   } while(j<=20);
				 ptr1_at[y]=0;
				 strcat(ptr1_at,"0008A0");//短信格式
				//_________短信长度、５个汉字，10个英文   _____________

            		i=(r/2);//PDU长度报文
            		j=(i/16);
            		if(j>9)
            		  ptr_tmp[0]=j+0x37;
            		else
            		 ptr_tmp[0]=j+0x30;

            		j=(i%16);
            		if(j>9)
            		  ptr_tmp[1]=j+0x37;
            		else
            		  ptr_tmp[1]=j+0x30;
     				ptr_tmp[2]=0;
				   strcat(ptr1_at,ptr_tmp);
				// strcat(ptr1_code,"0A");  //
				 //---------------------------------------
				//短信内容
				//53D1 9001 6210 529F FF01 发送成功
			//     strcat(ptr1_code,"53D190016210529FFF01\x1a");
                 strcat(ptr1_at,ptr1_code);
                 strcat(ptr1_at,"\x1a");


                Send_AT_Command(AT_COMMAND,ptr1_at,0);
				 return(i);

}
/*
//===================================================
// 发送短消息内容方式
//====================================================
#define   	smss_text_1    	1
#define 	smss_text_tmp	2
#define		smss_pud_1		3
#define		smss_start		4
#define		smss_pud_1		5
 */
/*
*********************************************************************************************************
** 函数名称 ：send_sms()
** 函数功能 ：GSM模块发送短信
** 入口参数 ：ptr --at_buf AT指令发送用 ptr1_code  短信内容,ptr_tel,TEL_temp 电话号码,
**            ID -暂时参数  可以转向发短信方式
   全局 变最     receive_count
** 出口参数 :
*********************************************************************************************************
*/ // //smss_text_1 smss_text_tmp   smss_pud_1 smss_start   smss_pud_1
//uint8 send_sms( char* ptr1_at,char* ptr1_code, char* ptr_tel,uint8 id)
uchar  send_sms(char* ptr1_at,char* ptr1_code,char* ptr_tel,uchar id) //发送短信
{  	uchar  j=1,i=1;

       Send_AT_Command(AT_CMGF0,ptr1_at,0); //ATE0V0^SSMSS=1+CNMI=2,1
		switch(id)
		{
		    case	smss_text_1:  //  发送英文

                     Send_AT_Command(AT_CMGF1,ptr1_at,0);
                      Send_AT_Command(AT_CMGF1,ptr1_at,0);
                        //AT+CMGS=   "13818120592"    >
                     i=0;
                     ptr1_code[i++]='"';
                     j=0;
   	  				 do{
   	       				   ptr1_code[i++]=ptr_tel[j++];
                           if(ptr_tel[j]==0x0d||ptr_tel[j]==0)
                             {
                             	ptr1_code[i++]='"';
                                ptr1_code[i++]=0;
                                break;
                               }

   	   				 }while( i<=19);

					Send_AT_Command(SMS_CMGS,ptr1_at,ptr1_code);
		    	    Send_AT_Command(CMGS_MUB,ptr1_at,"SEND: OK!\x1a");
                break;

		 case	smss_text_tmp:  // 专用回复  TEL_temp
                        Send_AT_Command(AT_CMGF1,ptr1_at,0);
                        //AT+CMGS=   "13818120592"    >
                        strcpy(ptr1_at,"AT+CMGS=\"");    //AT+CMGS= "13818120592"
        				strcat(ptr1_at,ptr_tel);
        				strcat(ptr1_at,"\"");
        				Send_AT_Command(AT_COMMAND,ptr1_at,0);
		    	      Send_AT_Command(CMGS_MUB,ptr1_at,ptr1_code);
                break;

          case smss_pud_2:         //发送中文
               //0011000D91683118180295F20008A70A8F66670953719669FF01

                  //53D1 9001 6210 529F FF01 发送成功
			     strcpy(ptr1_code,"53D190016210529FFF01");
                 i=pdu_set(ptr1_at,ptr1_code,ptr_tel);
				   //短信号码长度，如果每加一个中文，长度加２
               /* Send_AT_Command(SMS_CMGS,ptr1_at,"25");

                y=0;
                strcpy(ptr1_code,"0011000D9168");
                y=12;
				j=0;  //发送对方号码
				 //对方手机号码 进行 PDU转换
			   // 将TEL_temp里的 13818120592转换3118180295F2后放到 para_temp里
					do
					{
				      ptr1_code[y++]=ptr_tel[j+1];
				      ptr1_code[y++]=ptr_tel[j];
				      j=j+2;
				      if(ptr_tel[j+1]==0x0d||ptr_tel[j+1]==0)
				      { ptr1_code[y++]='F';
				      	ptr1_code[y++]=ptr_tel[j];
				      	break;
				      }
				   } while(j<=18);
				 ptr1_code[y]=0;
				 strcat(ptr1_code,"0008A0");//短信格式
				 strcat(ptr1_code,"0A");  //短信长度、５个汉字，10个英文
				//短信内容
				//53D1 9001 6210 529F FF01 发送成功
			     strcat(ptr1_code,"53D190016210529FFF01\x1a");

                Send_AT_Command(CMGS_MUB,ptr1_at,ptr1_code);
 				*/
          break;

       case smss_pud_1:
              //0011000D91683118180295F20008A70A8F66670953719669FF01
                  //8BBE 7F6E 6210 529F 0021   设置成功!
			     strcpy(ptr1_code,"8BBE7F6E6210529F0021");
                 i=pdu_set(ptr1_at,ptr1_code,ptr_tel);
               /*
				   //短信号码长度，如果每加一个中文，长度加２
                Send_AT_Command(SMS_CMGS,ptr1_at,"25");

                y=0;
                strcpy(ptr1_code,"0011000D9168");
                y=12;
				j=0;  //发送对方号码
				 //对方手机号码 进行 PDU转换
			   // 将TEL_temp里的 13818120592转换3118180295F2后放到 para_temp里
					do
					{
				      ptr1_code[y++]=ptr_tel[j+1];
				      ptr1_code[y++]=ptr_tel[j];
				      j=j+2;
				      if(ptr_tel[j+1]==0x0d||ptr_tel[j+1]==0)
				      { ptr1_code[y++]='F';
				      	ptr1_code[y++]=ptr_tel[j];
				      	break;
				      }
				   } while(j<=18);
				 ptr1_code[y]=0;
				 strcat(ptr1_code,"0008A0");//短信格式
				 strcat(ptr1_code,"0A");  //短信长度、５个汉字，10个英文
				//短信内容
				//8BBE 7F6E 6210 529F 0021   设置成功!
			     strcat(ptr1_code,"8BBE7F6E6210529F0021\x1a");
                Send_AT_Command(CMGS_MUB,ptr1_at,ptr1_code);
                       */
          break;


		//======================发送收到设备开启指令================================================

		  default:

			break;

    }


      	  receive_count=0;
      	  timer_S_cnt=0;
      	  while(timer_S_cnt<15 )
       		{   if(strsearch("+CMGS:",ptr1_at))   break;
       		}
          for(i=0;i<=MAX_TM-2;i++) {   ptr1_at[i]=0;	}
        //  num_tmp1为短信号, 从16进制转成 文本方式才能发送出去
 	    //短信号码删除 AT+CMGD=01
		 CMGD_GSM(ptr1_at,  sms_num_tmp );
         Send_AT_Command(AT_CMGF0,ptr1_at,0); //ATE0V0^SSMSS=1+CNMI=2,1

         return(0);

}

/*
*********************************************************************************************************
** 函数名称 	 PHONE_RD()
** 函数功能		  进行SIM卡的电话本读
** 全局变量或数组:   receive_count 接收指针
** 入口参数 	 ：  AT指令 字符串
				 	str_at- uart_buff   str_code -para_temp
**            	 	*str_at=要复制的内容/转AT指令
**            	 	*str   电话本内容
                     id 电话本号
** 出口参数 	： 1-成功      0-失败
*********************************************************************************************************
*/

//uart_buff,para_temp,read_tmp
uchar PHONE_RD(char* ptr1_at,uchar *str,uchar id )      //read_tmp=0x30+i;  要读的电话号码
{       uchar idata j;
        uchar  idata ptr[3],i=0;;
     	i=0;
     	ptr[i++]=(id/10)+0x30;
     	ptr[i++]=(id%10)+0x30;
     	ptr[i++]=0;
     	Send_AT_Command(PHONE_READ,ptr1_at,ptr);
     	timercount=0; while(timercount<10);
         i=0;
     	if(j=strsearch("+CPBR:",ptr1_at))    //+CPBW: 1,"13818120592",129,"chia"
     	{ 	j=j+9;
       		do
        	{
   			 	str[i++]= ptr1_at[j++];
   				if( ptr1_at[j]=='"')
   				{ str[i++]=0;
   				   return(i);
   			   	}


   	    	}while(j<=24);


       }
       else if(ptr1_at[1]==0x0d&&ptr1_at[0]==0x34)
           {           return(FALSE);
           	}
        else return(FALSE);
}

/*
*********************************************************************************************************
** 函数名称 	 READ_TEL()
** 函数功能		   进行SIM卡中的1到20条短信号码的读取
** 全局变量或数组:   receive_count 接收指针
** 入口参数 	 ：  AT指令 字符串
				 	str_at- uart_buff   str_code -para_temp
**            	 	*str_at=要复制的内容/转AT指令
**            	 	*str_code  短信内容
                     id 短信号
** 出口参数 	： 1-成功      0-失败
*********************************************************************************************************
*/
//======================================================================
//======================================================================
//======================================================================
uchar   READ_TEL(char* ptr1_at,uchar  id)                //读短信号码１－２０个
{

	    uchar  idata ptr1[5],t;
     	t=0;
     	ptr1[t++]=(id/10)+0x30;
     	ptr1[t++]=(id%10)+0x30;
     	ptr1[t++]=0;
	 Send_AT_Command(SMS_CMGR,ptr1_at,ptr1);
      timercount=0; while(timercount<30);
     //             +CMGR: 1
    if(t=strsearch("+CMGR:",ptr1_at))
	// if(receive_count<10)
	 {          return(1);
	  }   //+CMS ERROR
     else  if(t=strsearch("+CMS ERROR",ptr1_at))
     {
                return(4);
     }
      return(0);

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
	uchar i,j;
       ptr1_code[0] =  ptr1_code[0];
     BELL=0;  for(i=0;i<=100;i++) dmsec(1);  BELL=1;


	for(i=0;i<12;i++)
	{   //进行初始化,并显示  ATE0V0  = ATE0 ATV0
		Send_AT_Command(TC35_INIT,ptr1_at,0);
		if(ptr1_at[1]==0x0d&&ptr1_at[0]==0x30)
		{  break;}
        else if(ptr1_at[receive_count-1]==0x0d&&ptr1_at[receive_count-2]=='0')
         {  break;}
		else if(i>1)
		 	{ IO_IGT=0;
			  timer_S_cnt=0; while(timer_S_cnt<2);
			   	IO_IGT=1;
			  timer_S_cnt=0; while(timer_S_cnt<2);
			  	Send_AT_Command(TC35_INIT,ptr1_at,0);
	
		 	}
	/*	 if((i%4)==0)
		{
			IO_IGT=1;  //点火
		}
		else if((i%2)==0)
		{
			IO_IGT=0;
		}  */

   	  for(j=0;j<=20;j++)   dmsec(1);

    }
    if(i>=11)  //模块有问题
       goto at_eer;


	for(i=0;i<12;i++)
    	{            //读卡  AT+CPIN?
    		if(Send_AT_Command(AT_CPIN,ptr1_at,0))
    		{//如有收到回应进行分析收到的信息,
				ptr1_at[receive_count]=0;
				if(strsearch("READY",ptr1_at)!=0)
        			break;
        	}
            timer_S_cnt=0; while(timer_S_cnt<2);
			  
    	}
   if(i>=11)
    {
at_eer:   Send_AT_Command(RESET_TC35,ptr1_at,0); //模块重启  AT+CFUN=1,1
            for(j=0;j<=2;j++)  for(i=0;i<=200;i++) dmsec(1);
          Send_AT_Command(AT_CMGF0,ptr1_at,0); //ATE0V0^SSMSS=1+CNMI=2,1
	      for(j=0;j<=20;j++)  for(i=0;i<=200;i++) dmsec(1);
    }


  	Send_AT_Command(AT_CMGF0,ptr1_at,0);//ATE0V0^SSMSS=1+CNMI=2,1
 	Send_AT_Command(AT_IPR,ptr1_at,0);//波特率  AT+IPR=9600
  	Send_AT_Command(AT_CGMM,ptr1_at,0);  //模块型号 AT+CGMM
  	//_______________GPRS_________
  	Send_AT_Command(AT_CIPHEAD,ptr1_at,0);  //设置接收数据的IP 头
    Send_AT_Command(AT_CDNSORIP,ptr1_at,"1"); //域名方式
   // strcpy(ptr1_code,"\"222.73.49.102\"\x00");
   // Send_AT_Command(AT_CDNSCFG,ptr1_at,ptr1_code);   //域名服务器
	Send_AT_Command(AT_CIPSHUT,ptr1_at,0);   //断开连接
  	//-----------------------

    for(j=0;j<=60;j++)  for(i=0;i<=200;i++) dmsec(1);
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


   #if   CPU_TYPE2==W78E58
    //--------W78LE专用-开启外部RAM-------------
    CHPENR=0x87;
    CHPENR=0x59;        //enable CHPCON write option
    CHPCON=CHPCON|0x10; //enable AUX-RAM,use save programe data
    //---------------------
    #elif   CPU_TYPE2==STC89E58
      serial_port_two_initial();

   #endif
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
	uint   i;

    switch(type)
    {

	case VOICE_DIAL:   	//语音拨号
        strcpy(str_at,"ATD");
        strcat(str_at,str_code);
      // strcpy(uart_buff,"ATD10086;");
        strcat(uart_buff,";");

        break;
      case RESET_TC35:		//复位TC35
      	strcpy(str_at,"ATE0V0+CFUN=0;+CFUN=1");
     	break;
      case AT_CPIN:		//检查当前是否要输入PIN码
        strcpy(str_at,"AT+CPIN?");
        break;

      case TC35_INIT:		//TC35初始化命令     ATE0 ATV0 合体
        strcpy(str_at,"ATE0V0");
        break;
        //0D 0A 54 43 33 35 0D 0A
      case   AT_CGMM:   //模块版型号
        strcpy(str_at,"AT+CGMM");
        break;

      case AT_CMGF0:    // 也是合体组合  ATE0 ATV0 AT+CMGF=1 AT+CNMI=2,1
          strcpy(str_at,"ATE0V0+CMGF=0;+CNMI=2,1");
        break;
       case  AT_IPR:   // 波特率 9600
        strcpy(str_at,"AT+IPR=9600&W"); break;
		case CALL_ID:			//读来电显示
      	strcpy(str_at,"AT+CLCC");
      	break;
        case SMS_CMGL:		//删除一个短信
        // strcpy(uart_buff,"AT+CMGL=\"ALL\"");
        strcpy(str_at,"AT+CMGL=4");
      //  strcat(uart_buff,ptr);
        break;
      case HOOKOFF:			//挂机,停止通话
        strcpy(str_at,"ATH");
        break;
      case AT_CMGF1:	//SMS 接收方式,
        strcpy(str_at,"ATE0V0+CMGF=1;+CNMI=2,1");
        break;

      case PHONE_WRITE:  	//存电话号码
        strcpy(str_at,"AT+CPBW=");
        strcat(str_at,str_code);
        break;
      case PHONE_READ:  	//读电话本
        strcpy(str_at,"AT+CPBR=");
        strcat(str_at, str_code);
        break;
      case SMS_CMGR:		//读一个短信
        strcpy(str_at,"AT+CMGR=");
        strcat(str_at,str_code);
        break;
       case SMS_CMGS:		//写一个短信
        strcpy(str_at,"AT+CMGS=");    //AT+CMGS= "13818120592"
        strcat(str_at,str_code);

        break;
       case SMS_CMGD:		//删除一个短信
        strcpy(str_at,"AT+CMGD=");
        strcat(str_at,str_code);
        break;
       case  CMGS_MUB:    //发报警短信内容
        strcpy(str_at,str_code);
         break;
       case AT_CPMS:
      	 strcpy(str_at,"ATE0V0+CPMS=\x22SM\x22");
        break;  //AT_CPMS
       case PICK_UP:         //摘机
	    strcpy(str_at,"ATA");
     //----------------------------------------------------
      //--------------------------------------------------------

         case AT_COLP:		//电话方式
        strcpy(str_at,"AT+COLP=1");
        break;
	     case AT_CLTS:		//检查当前是否要输入PIN码
        strcpy(str_at,"AT+CLTS");
        break;

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
            // strcat(uart_buff,&str_code);
             break;
        case AT_CIPATS:        //AT+CIPATS 设置自动发送时间
            strcpy(str_at,"AT+CIPATS=1,50");
           //  strcat(uart_buff,str_code);
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
            // AT+CIPSTATUS
         case AT_CIPSTATUS:
            strcpy(str_at,"AT+CIPSTATUS");

             break;
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
    for(i=0;i<254;i++)
	{	if(str_at[i]==0) break;

		SBUF=str_at[i];
		while(!TI);
		TI=0;
		if(str_at[i]==0x1a)  break;
	}
   //---------------------
   receive_count=0;
   str_at[0]=0; str_at[1]=0;str_at[2]=0;
   i=0;
   do{
       dmsec(10);
       if(receive_count>1)
       {   	dmsec(1000);
       		//dmsec(100);
            break;
       }
  #if   CPU_TYPE2==W78E58
            }while(i++<1000);
   #elif  CPU_TYPE2==STC89E58
            }while(i++<1800);
  #endif


  /*  for(i=0;i<=100;i++) dmsec(1);
    for(i=0;i<=250;i++)
    {    dmsec(5);
         if(str_at[receive_count-1]==0x0d)
         {  dmsec(10);
            break;
         }
    } */

    return(TRUE);



}

/*************************************************************/
/*
*********************************************************************************************************
** 函数名称 	 tel_diat()
** 函数功能		     拨打前1个电话,时间设为80秒,
** 全局变量或数组:   receive_count 接收指针
** 入口参数 	 ：  AT指令 字符串
				 	str_at- uart_buff   str_code -para_temp
**            	 	*str_at=要复制的内容/转AT指令
**            	 	*str_code  发送的内容
** 出口参数 	：
*********************************************************************************************************
*/

void tel_diat(char* ptr1_at,char* ptr1_code)
{    uchar ii,j,t,r=0;
   //可以用一个电话
   //为没有打通电话时做临时存储  0000 0111, 可以拨打3个电话,分别是1,2,3
    t=0x01;  ii=0;
      do
        {   ii=ii+1;
			if(ii>=3)  ii=0;   // 最大值,不能起过三个电话
			if(t==0)   return ; //号码全部拨完返回

   			if(	(t>>ii-1)&0x01!=0)  //查t位是否有1
   			{ read_tmp=ii; }
   			else  { 	  continue;  }
   			j=PHONE_RD(ptr1_at,ptr1_code,read_tmp);    //读电话号码
            r++;
  			if(j>5)  //有号码会存入 para_temp  ,j是传来的指针,指向para_temp里的号码的最后一位+1
   			{
   				ptr1_code[j++]=';';
   				ptr1_code[j++]=0;
   				ptr1_code[j++]=0x00;
              if(!Send_AT_Command(VOICE_DIAL,ptr1_at,ptr1_code)); //拨号
                {    //拨号等待
    					receive_count=0;
   				   		timer_S_cnt=0;
   				   		while(timer_S_cnt<40 )
						{  if(((ptr1_at[0]=='0')||(ptr1_at[0]=='6'))&&ptr1_at[1]==0x0d)
						   break;
						}
   				 }
   				   if((((ptr1_at[0]=='0')&&ptr1_at[1]==0x0d)))      //拨号成功
   				   {
						 t&=~(1<<ii-1) ;  //清掉拨号成功的号码
    					receive_count=0;
   				   		timer_S_cnt=0;
   				   		while(timer_S_cnt<80)
   				   		{    if(!io_p00_on )
         						 { io_p00_on=1; BELL=0;
         						   timercount=0;  while(timercount<10) watch_dog_clear;//
         						    BELL=1;
         						  }
   				   			   if(!io_p02_on )
         						 { io_p02_on=1; BELL=0;
         						   timercount=0;  while(timercount<10) watch_dog_clear;//
         						    BELL=1;
         						  }
                               else if(!io_p03_on) //按键 挂机
         						{ io_p03_on=1;
         						   break;
         						 }
   				   		 };//等待挂机
	                     Send_AT_Command(HOOKOFF,ptr1_at,0);  //自动挂机
	                     if(t==0)   return ; //内容空返回
   				    }


   		 		  Send_AT_Command(HOOKOFF,ptr1_at,0); //挂机重拨


   		  }



   		}while(r<30); //最大拨号次数


}
/*************************************************************/

 /*
*********************************************************************************************************
** 函数名称 GPS_READ()
** 函数功能 ： GPS数据读取
** 入口参数 ：ptr1_at --uart_buff AT指令发送用 ptr1_code -para_temp 参数内容
               id   id为0是自己定义的一种格式发送 ,为1是标准原始格式发送
**             全局 变最    ptr1_at, uart_buff, receive_count

** 出口参数 :
*********************************************************************************************************
*/

void GPS_READ(char* ptr1_at,char* ptr1_code,char* ptr_tel,uchar id)
{   // uchar idata ptr_tmp[32];
	 uint y;
	 uchar  i,j,t,r,x;
    // LCD_INT();

	 GPS_ON=0;
	for(x=0;x<=10;x++)
	{
	   y=0;
	   receive_count=0;
	    do{
           dmsec(3);
          if(receive_count>1)
          {   	dmsec(50);
            break;
          }
        }while(y++<1200);
	//---$GPRMC,020041.000,V,3112.1447,N,12139.3450,E,1.25,282.95,050309,,,E*7A
	//得到代码, V,数据是否有效   3112.1447,N 进行转换通用码
	 //其中3112改成31 12不变  1447要转换成1447*6/10= 0868
	   if(j=strsearch("$GPRMC,",ptr1_at))
	   {  j=j+5;//指向时间
	       t=0;
	       do{
	          j++;
	          if(ptr1_at[j]==',')//结束
	           break;
	       }while(t++<13);
          j++;//A或V  A为有效
          t=0;
          ptr_tel[t++] =ptr1_at[j++];
        //---------------------------
          //j++;//指向 纬度
           ptr_tel[t++] =ptr1_at[j++];//,

          do{
	          ptr_tel[t++]=ptr1_at[j];
	          j++;
	          if(ptr1_at[j]==',')//结束
	           break;
	       }while(t<13);
	     //  ptr_tel[t++]='-';

      //--------纬度半球N(北半球)或S(南半球)---------------------
           // j++;//指向
           ptr_tel[t++]=ptr1_at[j++];    //内容是 ","
           //--------------------------------
           ptr_tel[t++]=ptr1_at[j++];   //内容是 N或S
           //-------------------三个空格,自己加的,为了显示用-----------------
           ptr_tel[t++]=' '; ptr_tel[t++]=' ';ptr_tel[t++]=' ';
          // ptr_tel[t++]=0;
          //  PLAY_LCD(0,0,ptr_tel); //第一行显示
      //---------------------------------
           j++;//指向 经度
         // t=0;
         r=0;
          do{
	          ptr_tel[t++]=ptr1_at[j];
	          j++;
	          if(ptr1_at[j]==',')//结束
	           break;
	       }while(r++<11);
	      // ptr_tel[t++]='-';

      //--------经度半球E(东经)或W(西经) )---------------------
            //  j++;//指向
              ptr_tel[t++]=ptr1_at[j++];  //内容是 ","
            //-------------------
             ptr_tel[t++]=ptr1_at[j++];   //内容是 E或W
            //========
          // ptr1_code[t++]=ptr1_at[j++];
           //ptr_tel[t++]=' '; ptr_tel[t++]=' '; ptr_tel[t++]=' ';
          //----------------------------
          //-----------------------------
           ptr_tel[t++]=0;
         //    PLAY_LCD(0,0,ptr_tel);  //显示代码

     if(id==0)     //自己定义格式  V,3112.1447,N,12139.3450,E,
     {
      //------------------------------------------------
      //-- 可以进行两种GPS 协议转换 ,-------------------------
      //------------------------------------------------
         //V,3112.1447,N  变成N31 12'14,47'
         //  12139.3450,E  变成E121 39'34.50'
            i=0; t=0;
			//----------------
			ptr1_code[i++]=ptr_tel[t++];//V
			ptr1_code[i++]=',';
			//-----------------
			i++;  //2 用来放 N位置
			t++;
			//-----------------
			//进行位数锁定,有四位与5位之分
            if(ptr_tel[t+4]=='.')   //  4位
            {          //3112四位  改成 31 12
              ptr1_code[i++]=ptr_tel[t++];//
              ptr1_code[i++]=ptr_tel[t++];//
              ptr1_code[i++]=',';//
              ptr1_code[i++]=ptr_tel[t++];//
              ptr1_code[i++]=ptr_tel[t++];//
            }
            else   //5位
            {
             ptr1_code[i++]=ptr_tel[t++];//
              ptr1_code[i++]=ptr_tel[t++];//
              ptr1_code[i++]=ptr_tel[t++];//
              ptr1_code[i++]=',';//
              ptr1_code[i++]=ptr_tel[t++];//
              ptr1_code[i++]=ptr_tel[t++];//
            }
            t++;
             ptr1_code[i++]=',';//.号
          //-------1447要转换成1447*6/10= 0868--------转换-----------
          //先转换成16进制,主要是好乘除
          ////------四位------------
			y=ptr_tel[t++]-0x30;
			y=y*10;
			y=y+ptr_tel[t++]-0x30;
			y=y*10;
			y=y+ptr_tel[t++]-0x30;
			y=y*10;
            y=y+ptr_tel[t++]-0x30;

           y=y*6;
           y=y/10;  //得到整数
           //---------------------------

                  // yy= yy%10000;
                  r= y/100 ;

                  ptr1_code[i++]=(r/10)+0x30;
                  ptr1_code[i++]=(r%10)+0x30;
                  ptr1_code[i++]=',';
                  r= y%100;
                  ptr1_code[i++]=(r/10)+0x30;
                  ptr1_code[i++]=(r%10)+0x30;
                  ptr1_code[i++]=',';
        //----------------------------------------------
              t++;  //,号不行
              ptr1_code[2]=ptr_tel[t++];   //3479
            //-----------------------
            //跳线 三个空格
             t=t+3;

            ////V,3112.1447,N   12139.3450,E
             j=i;
             i++;//为了存E
			if(ptr_tel[t+4]=='.')   //  4位
            {          //3112四位  改成 31 12
              ptr1_code[i++]=ptr_tel[t++];//
              ptr1_code[i++]=ptr_tel[t++];//
              ptr1_code[i++]=',';//
              ptr1_code[i++]=ptr_tel[t++];//
              ptr1_code[i++]=ptr_tel[t++];//
            }
            else   //5位
            {
             ptr1_code[i++]=ptr_tel[t++];//
              ptr1_code[i++]=ptr_tel[t++];//
              ptr1_code[i++]=ptr_tel[t++];//
              ptr1_code[i++]=',';//
              ptr1_code[i++]=ptr_tel[t++];//
              ptr1_code[i++]=ptr_tel[t++];//
            }
            t++;
             ptr1_code[i++]=',';//.号
          //-------1447要转换成1447*6/10= 0868--------转换-----------
          //先转换成16进制,主要是好乘除
          ////------四位------------
			y=ptr_tel[t++]-0x30;
			y=y*10;
			y=y+ptr_tel[t++]-0x30;
			y=y*10;
			y=y+ptr_tel[t++]-0x30;
			y=y*10;
            y=y+ptr_tel[t++]-0x30;
             y=y*6;
           y=y/10;  //得到整数3497
           //---------------------------

                  // yy= yy%10000;
                  r= y/100 ;

                  ptr1_code[i++]=(r/10)+0x30;
                  ptr1_code[i++]=(r%10)+0x30;
                  ptr1_code[i++]=',';
                  r= y%100;
                  ptr1_code[i++]=(r/10)+0x30;
                  ptr1_code[i++]=(r%10)+0x30;
                  ptr1_code[i++]=',';
        //----------------------------------------------
                t++;  //,号不行
              ptr1_code[j]=ptr_tel[t++];
              ptr1_code[i++]=0;
        //-----------------------
       }
       else  //标准格式原始数据  V,3112.1447,N,12139.3450,E,
       {
            strcpy(ptr1_code,ptr_tel);
          //PLAY_LCD(0,0,ptr1_code);
         /*  timercount=0; while(timercount<200) ;
           timercount=0; while(timercount<200) ;
           timercount=0; while(timercount<200) ;
            PLAY_LCD(0,0,"                                     ");
           PLAY_LCD(0,0,ptr_tel);  */
        }
        break;
	 }



	//  PLAY_LCD(0,1,ptr_tel);
    }
  //-------------------------------------

     GPS_ON=1;

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
uchar GPRS_SEND(char* ptr1_at,char* ptr1_code)
{  //uchar  i ;

	  Send_AT_Command(AT_CIPSEND,ptr1_at,0);      //发送GPRS数据
	  timercount=0; while(timercount<1);

	  if(ptr1_at[0]=='4'&&ptr1_at[0]==0x0d)
	    return(0);
      strcat(ptr1_code,"\x1a\x00");
      Send_AT_Command(CMGS_MUB,ptr1_at,ptr1_code);

      timer_S_cnt=0; while(timer_S_cnt<5)
      {
      	 if( strsearch("CLOSE",ptr1_at))  //SEND OK
            return(0);
      	 else if( strsearch("SEND OK",ptr1_at))  //SEND OK
      	    return(1);
       }
      return(0);

}

//-----------------

/*
*********************************************************************************************************
** 函数名称 	 GPRS_SET()
** 函数功能		GPRS中的 TCP 协议连接   这里用的是域名方式连接
** 全局变量或数组:   receive_count 接收指针
** 入口参数 	 ：  AT指令 字符串
				 	str_at- uart_buff   str_code -para_temp
**            	 	*str_at=要复制的内容/转AT指令
**            	 	*str_code AT指令的参数或其它参数
** 出口参数 	：
*********************************************************************************************************
*/
uchar  GPRS_SET(char* ptr1_at,char* ptr1_code,char* ptr_tel)
{

//	  uchar i;

	 Send_AT_Command(AT_CIPSTATUS,ptr1_at,0); //查询
	  timercount=0; while(timercount<1);
	if(ptr1_at[receive_count-5]==0x0d&&ptr1_at[receive_count-4]==0x0a
	   &&ptr1_at[receive_count-3]=='8'
	  &&ptr1_at[receive_count-2]==0x0d&&ptr1_at[receive_count-1]==0x0a
	 )
	 {   return(1) ;   //如果在网不用连接
	  }
	 else    //没有 在网
	 {
        Send_AT_Command(AT_CIPHEAD,ptr1_at,0);
	 	Send_AT_Command(AT_CIPCLOSE,ptr1_at,0); //关闭GPRS
		timercount=0; while(timercount<1);
		/***********************************/
		 if(ptr_tel[0]!=0)  //域名方式
		 {
		 	Send_AT_Command(AT_CDNSORIP,ptr1_at,"1"); //域名方式
    		//strcpy(ptr_tel,"\"222.73.49.102\"\x00");
    		 //域名服务器
    		//Send_AT_Command(AT_CDNSCFG,ptr1_at,ptr_tel);
    	 }
    	else  //IP方式
    	  Send_AT_Command(AT_CDNSORIP,ptr1_at,"0"); //IP方式
    	 //--------------------
    		//---------------------------------------
		//  strcpy(ptr1_code,"\"TCP\",\"llx1.xicp.net\",\"1001\"\x00");
		 //AT+CIPSTART="TCP","124.78.41.17","8080"

         Send_AT_Command(AT_CIPSTART,ptr1_at,ptr1_code); //连接GPRS
         /**************************************/
        //等待连网成功报告
		 timer_S_cnt=0; while(timer_S_cnt<60)
		{
		   if(ptr1_at[receive_count-5]==0x0d&&ptr1_at[receive_count-4]==0x0a
			&&ptr1_at[receive_count-3]=='8'
			&&ptr1_at[receive_count-2]==0x0d&&ptr1_at[receive_count-1]==0x0a
				)    //连结成功
           {
           	timer_S_cnt=0; return(1) ;
           }

           else
          if (ptr1_at[receive_count-5]==0x0d&&ptr1_at[receive_count-4]==0x0a
            &&ptr1_at[receive_count-3]<='7'
			&&ptr1_at[receive_count-2]==0x0d&&ptr1_at[receive_count-1]==0x0a
			 )    //失败退出
			{  Send_AT_Command(AT_CIPCLOSE,ptr1_at,0); return(0) ;  }
		 if(ptr1_at[receive_count-2]=='4'&&ptr1_at[receive_count-1]==0x0d)
		  {
		       Send_AT_Command(AT_CIPCLOSE,ptr1_at,0);  return(0) ;
		  }
	  }
      if(timer_S_cnt!=0) {Send_AT_Command(AT_CIPCLOSE,ptr1_at,0); return(0) ;  } //失败返回
	}



}

/*
*********************************************************************************************************
** 函数名称 	 GPRS_UP()
** 函数功能		GPRS中的 TCP 协议连接   这里用的是域名方式连接
** 全局变量或数组:   receive_count 接收指针
** 入口参数 	 ：  AT指令 字符串
				 	str_at- uart_buff   str_code -para_temp IP及端口
				 	ptr_tel  域名
**            	 	*str_at=要复制的内容/转AT指令
**            	 	*str_code AT指令的参数或其它参数
** 出口参数 	：
*********************************************************************************************************
*/
/***********************************************/
//-------GPRS联网及处理------------------------------------
//               uart_buff para_temp   receive_count
////char* ptr1_at,char* ptr1_code, char* ptr_tel
uchar  GPRS_UP(char* ptr1_at,char* ptr1_code,char* ptr_tel)
{
        uchar i,j;
   // GPRS TCP或UDP连接,本机采用TCP
	i=GPRS_SET(ptr1_at,ptr1_code, ptr_tel);
    if(i==0)  return(0) ;//失败返回
 	/**********************************************/
	//  以下为连接成功后处理
	/************************************************/
  //--------------------------------
         strcpy(ptr1_code,"SIM300 OK!");  //通知道对方已经连接
         if(GPRS_SEND(ptr1_at,ptr1_code)==0)  //发送GPRS数据
		    i=0;
         //    return ;     //失败退出
    //---------------------------
         P0=0;
       timer_S_cnt=0; while(timer_S_cnt<80) //联网后处理
       {
          if(receive_count>1) //收到对方数据进行处理
          {
              timercount=0; while(timercount<2);
              ptr1_at[receive_count]=0;
              i=strsearch("+IPD",ptr1_at); //IP头
            //+IPD5:LED3N  +IPD10:12345LED3N
            if(i)
            {
              receive_count=0;
             //-------报文长度,  这里跳过--------------
              i=i+3;
              i++;
              if(ptr1_at[i]==':')// :后面是内容
              i++;
              else i=i+2;
              j=i;
              //-下面GPRS数据处理区------------
           //---------LED2N LED3N LED4N LED5N LED6N LED7N--------------------------------
 		   //         LED2F LED3F LED4F LED5F LED6F LED7F
 			 if(ptr1_at[i]=='B'&&ptr1_at[i+1]=='E'&&ptr1_at[i+2]=='E'&&ptr1_at[i+3]=='P')
           {                 timer_S_cnt=0;
                  BELL=0;
     				for(i=0;i<4;i++)    //灯闪三下
     			{  dmsec(100);
    			 }
 				BELL=1; //蜂鸣器
 				 strcpy(ptr1_code,"BEEP OK!");
                 GPRS_SEND(ptr1_at,ptr1_code);
           }
         //  else  if(strsearch1(i,"START",ptr1_at))
         else if(ptr1_at[i]=='S'&&ptr1_at[i+1]=='T'&&ptr1_at[i+2]=='A'
                  &&ptr1_at[i+3]=='R'&&ptr1_at[i+4]=='T')

           {
                 IO_OUT=0; //继电器吸合
 				 strcpy(ptr1_code,"Relay ON OK!");
                 GPRS_SEND(ptr1_at,ptr1_code);  timer_S_cnt=0;
           }
         //   else  if(strsearch1(i,"END",ptr1_at))
          else if(ptr1_at[i]=='E'&&ptr1_at[i+1]=='N'&&ptr1_at[i+2]=='D')

           {
                 IO_OUT=1; //继电器吸合
 				 strcpy(ptr1_code,"Relay OFF OK!");
                 GPRS_SEND(ptr1_at,ptr1_code);   timer_S_cnt=0;
           }
          else  if(strsearch1(i,"GPS",ptr1_at))
           {
                 GPS_READ(ptr1_at,ptr1_code,ptr_tel,0);

                 GPRS_SEND(ptr1_at,ptr1_code);  timer_S_cnt=0;
           }
           /**********************************************/
           //--------SIM300 OFF--        对方关GPRS----------------
          /**************************************************/
           else if(ptr1_at[i]=='S'&&ptr1_at[i+1]=='I'&&ptr1_at[i+2]=='M'
                &&ptr1_at[i+3]=='3'&&ptr1_at[i+4]=='0'&&ptr1_at[i+5]=='0'
                && ptr1_at[i+6]==' '&&ptr1_at[i+7]=='O'&&ptr1_at[i+8]=='F'
                &&ptr1_at[i+9]=='F' )
				 {
				 	   Send_AT_Command(AT_CIPCLOSE,ptr1_at,0);
						   strcpy(ptr1_code,"SIM300 OFF OK!");
                           GPRS_SEND(ptr1_at,ptr1_code);
                       Send_AT_Command(AT_CIPHEAD,ptr1_at,0);   timer_S_cnt=0;
							      return(1) ;
				 }
			else {
		 	        #if   CPU_TYPE2==STC89E58
		 	            i=j;j=0;
		 	            do{
		 	               if(ptr1_at[i] ==0) break;
		 	                pc_buff[j++]=  ptr1_at[i++];

		 	            }while(j<=180);
                         pc_buff[j++]=0;
		 	            // strcpy(pc_buff,ptr1_at);
                 	//	 strcat(pc_buff,"\x0d");
                 		 send_UART_two(pc_buff);
		 	           #endif

		 	      receive_count=0;
		 	  }

		 }

		     else {

		 	      receive_count=0;
		 	  }



        } /// 收到的数据
      /*********************************************/
		 else if(!io_p00_on )   //硬件
         { io_p00_on=1;   //中文 D6D0CEC4
            strcpy(ptr1_code,"\xD6\xD0\xCE\xC4 OUT2 OK!");
            GPRS_SEND(ptr1_at,ptr1_code);
         }
         else if(!io_p02_on) //硬件
          { io_p02_on=1;
            strcpy(ptr1_code,"\xD6\xD0\xCE\xC4 OUT1 OK!");
            GPRS_SEND(ptr1_at,ptr1_code);
         }
        else if(!io_p03_on) //硬件
         { io_p03_on=1;
           strcpy(ptr1_code,"SIM300 OFF OK!");
           GPRS_SEND(ptr1_at,ptr1_code);
           	Send_AT_Command(AT_CIPCLOSE,ptr1_at,0); //关闭GPRS
				 return(1) ;
         }
       #if   CPU_TYPE2==STC89E58
                       /* 第二串口 ,就是双串口单片机才有的控制*/
				else	if(pc_bit==1)
					   {    timercount=0; while(timercount<5);
					 		pc_bit=0; pc_count=0;
                                 timer_S_cnt=0;

                           GPRS_SEND(ptr1_at,pc_buff);
                       Send_AT_Command(AT_CIPHEAD,ptr1_at,0);

					        for(i=0;i<=100;i++)
					        {
					            pc_buff[i]=0;
					        }

					   }
             #endif


     /*****************************************/
         else  if(receive_count>10)
        {
        	receive_count=0;
        }
    }

    return(2) ;

}
/****************************************/
/*
// system_server
#define	SYS_PDU_1					1	//
#define	SYS_RING					2	//
#define SYS_TEXT_1					3	//
#define SYS_KEY_1						4	//
#define	SYS_SMSR					5	//
#define SYS_KEY_2					6	//
#define SYS_SMSS					7	//
#define SYS_TEL						8    //
#define SYS_DIAL                    9
*/
/******************************************/
void main(void)
{
	uchar i;
	Sys_Init();
	Initialize_Model(uart_buff,para_temp);//进行初始化
	P0=0;

	 system_server=SYS_SMSR;
	//-----------------------
	/*system_server=SYS_TEXT ;
	receive_count=0;
	timer_S_cnt=0; while(timer_S_cnt<15)*/
	//---------------------
                  strcpy(pc_buff,"SIM300 OK!");
                  strcat(pc_buff,"\x0d");
                  send_UART_two(pc_buff);

    //  p1_bit_on=p1_bit_off=p2_bit_on=p2_bit_off=0;

    pc_count=0;
	receive_count=0;
	sms_tmp=0;
    PT2272_BIT=0;PT2272_TMP=0;// 参数初始化
    io_p00_on=1;io_p02_on=1;io_p03_on=1;
	io_p02_bit =0;

	while(1)
	{

		//-----------查询1到20条中的短信,只要有短信就处理------------------------------------------
		if(system_server==SYS_SMSR)			//读1-20条短信
			{
                system_server=SYS_SMSR;
				i=read_sms(uart_buff,para_temp,TEL_temp,GPRS_TMP);
				if(i==0)
				{
				    receive_count=0;   timercount=0; while(timercount<5);
                     receive_count=0;
		     		timer_S_cnt=0; while(timer_S_cnt<60)
		          	{
		              //-----按键 呼入  ----------------
		               i=P2_INIT() ;
		               if(i!=0)
		                {    system_server=i;
		                	 break;
		                 }
                      //-------短信--------------
					   if(strsearch( "+CMTI:",uart_buff))
					     {  timercount=0; while(timercount<15);
					     break; }

					/*****流水流与 PT2272处理**********************/
					 //  LED_1();
                     #if   CPU_TYPE2==STC89E58
                       /* 第二串口 ,就是双串口单片机才有的控制*/
					   if(pc_bit==1)
					   {    timercount=0; while(timercount<5);
					 		pc_bit=0; pc_count=0;
                             if((pc_buff[0]=='a'||pc_buff[0]=='A')&&pc_buff[1]==0x0d)
                             {
                                  P0_1=1;

                                       strcpy(pc_buff,"OUT1 ON!");
                 					 strcat(pc_buff,"\x0d");
                 				 send_UART_two(pc_buff);

                             }
                             else
                            if((pc_buff[0]=='b'||pc_buff[0]=='B')&&pc_buff[1]==0x0d)
                            {
                            	P0_1=0;

                                     strcpy(pc_buff,"OUT1 OFF!");
                 					 strcat(pc_buff,"\x0d");
                  					send_UART_two(pc_buff);


                          }

                           if((pc_buff[0]=='c'||pc_buff[0]=='C')&&pc_buff[1]==0x0d)
                            {
                            	P0_2=1;
                                strcpy(pc_buff,"OUT2 ON!");
                 				strcat(pc_buff,"\x0d");
                  					send_UART_two(pc_buff);


                            }


                             if((pc_buff[0]=='d'||pc_buff[0]=='D')&&pc_buff[1]==0x0d)
                            {
                            	P0_2=0;

                                     strcpy(pc_buff,"OUT2 OFF!");
                 					 strcat(pc_buff,"\x0d");
                  					send_UART_two(pc_buff);


                            }


					        for(i=0;i<=100;i++)
					        {
					            pc_buff[i]=0;
					        }

					   }
                       #endif


		          	}


				}


            }
 	//------------以下是短信处理-------------------------------------
 			// 硬件中断处理 连GPRS 服务器
		  else if(system_server==SYS_KEY_2)

		  {
		  		//   连GPRS 服务器    这里可能更改自己 的IP或域名
		  	   //-----------------域名方式--用下面两条-----------------------
                 // LLXDZ.VICP.NET   域名   1001 端口
				//域名及端口
		 //	 strcpy(para_temp,"\"TCP\",\"LLXDZ.VICP.NET\",\"1001\"\x00");
		  	    //花生壳域名服务器,提供hificat.vicp.net域名
		 // 	 strcpy(GPRS_TMP,"\"222.73.49.102\"\x00");

			 ////-------------------------------------------


			  //---------------IP方式-用下面两条-------------------------
			   //-    // 114.50.52   域名   1001 端口
				//58.60.207.182  端口5005
				strcpy(para_temp,"\"TCP\",\"122.234.243.143\",\"1001\"\x00");
 			 	GPRS_TMP[0]=0;

			  //为0 才能进入IP方式连接

		  	//----------------------------------------


  			 GPRS_UP(uart_buff,para_temp,GPRS_TMP);
		     system_server=SYS_SMSR;

		 }
		 // 硬件中断处理   //发英文
		 else if(system_server==SYS_KEY_1)
		  {		//int_p03(uart_buff,para_temp,TEL_temp);
		  	      int_p02(uart_buff,para_temp,TEL_temp);
		          system_server=SYS_SMSR;

		  	}
	 //---------------------------------------------
		//======== // //回中文短信处理= ==========
			else if(system_server==SYS_PDU_1)
		  	{
		  		 send_sms(uart_buff,para_temp,TEL_temp,smss_pud_1);
				system_server=SYS_SMSR;
		  	}

         //-------------英文短信的回复---------------------------
		  else if(system_server==SYS_TEXT_1) //发送 para_temp里的号码
		  	{
		  		 send_sms(uart_buff,para_temp,TEL_temp,smss_text_tmp);
				system_server=SYS_SMSR;
		  	}

        //---------------//来电处理-------------------------
           else if(system_server==SYS_RING)
		  {
		  		 ring_auto(uart_buff,para_temp,TEL_temp);
		  		 system_server=SYS_SMSR;

	      }
	   //-----------------//打电话处理----------------------------
	     else if(system_server==SYS_DIAL)
		  {
 			   tel_diat(uart_buff,para_temp);
			   system_server=SYS_SMSR;
		  	}
      //---------------------------------------

	else  system_server=SYS_SMSR;

	}


}





























































