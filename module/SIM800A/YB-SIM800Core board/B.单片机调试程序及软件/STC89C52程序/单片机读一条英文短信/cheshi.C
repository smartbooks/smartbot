//==**********************************************************
//--软件功能, 读一条英文短信，但不回复，  START  END  两条，可以控制继电器
//---   可使用单片机  要求 16K ROM 256 RAM 及以上 8位标准单片机   STC SST SM W78E单片机
//--   TC35 SIM300 GTM900 EM310   波特率 9600
//100%不支持  AT89S51 W78E51及其它RAM为 128B的单片机
//============================================================
#include <reg52.h>
#include <cheshi.h>
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
//		#if   CPU_TYPE2==W78E58
       //    i =115;
        //#elif  CPU_TYPE2==STC89E58
           i =300;
        //#endif

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
        	k=i;
        	//for(k=i;k<MAX_TM-2;k++,j++)
        	do {
        		if(ptr2[j]==0)//比较正确
        			return(i+1);               //返回值是整数，不含0
        		if(ptr1_at[k]!=ptr2[j]) break;
        		k++;j++ ;
        	} while(k<MAX_TM-2) ;

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
       		if(ptr1[k]==ptr2[j])
       		{j++;k++;}
       		else return(0);
       		if(ptr2[j]==0)  return(k);
        }while(j<=60);
        return(0xff);
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

** 出口参数 	： 1-成功      0-失败
*********************************************************************************************************
*/

uchar CPMS_SM(char* ptr1_at )
//----------------------
{   uchar i,j,r,num_tmp1;
      Send_AT_Command(AT_INIT,ptr1_at,0);
      Send_AT_Command(AT_E,ptr1_at,0);
      Send_AT_Command(AT_CPMS,ptr1_at,0);
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

				   if(i=strsearch("+CMGL:\x20",ptr1_at))  //查短信代码回复是否对
				    {  i=i+6;
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
			if(i) //有短信跳出
				  {    i=i+7;
				       sms_num_tmp=num_tmp1;
				       return(1);
				  }

				}

           		r--;
           		num_tmp1++; //
       }while(r!=0);
      //---------------------------------------------
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
uchar  read_sms(char* ptr1_at,char* ptr1_code,char* ptr_tel)
{

	 uint  i;
     uchar idata  j,  PDU_TEXT;
     if(!CPMS_SM(ptr1_at))
     {
     		return(0);
     }



			        // 英文模式
			      //+CMGR: "REC READ","8613818120592",,"09/06/25,14:16:33+32",145,4,0,0,"8613800210500",
					//145,3
					//END  0
			     //  2B 43 4D 47 52 3A 20 22 52 45 43 20 52 45 41 44 22 2C 22 38 36 31 33 38 31 38 31 32 30 35 39 32 22
			     //2C 2C 22 30 39 2F 30 36 2F 32 35 2C 31 34 3A 31 36 3A 33 33 2B 33 32 22 2C 31 34 35 2C 34 2C 30 2C 30
			     // 2C 22 38 36 31 33 38 30 30 32 31 30 35 30 30 22 2C
				 //31 34 35 2C 33 0D 0A 45 4E 44 0D 0A 30 0D
                    //READ","
                     if((i=strsearch("READ\",\"",ptr1_at)))
                     {      i=i+6;

                              for(j=0;j<20;j++)
				  			 {
								ptr_tel[j]=uart_buff[i++];
								if(uart_buff[i]==0x22)   break;
				   			}
            	  			ptr_tel[j+1]=0;     ptr_tel[j+2]=0x00;

                     }
			            //AT_CMGF1

                         timercount=0; while(timercount<8)  ;
                         //从短信代码中找到维一代码与内容间的字符
                         if((i=strsearch("\x0d\x0a",ptr1_at)))
     						{     i=i+1;//指针移动两位,指向内容的第一个字符
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




  //------------------------------------------------------------
        i=0;
	   if(strsearch1(i,"START",ptr1_code))
	   {


//		     	 	IO_OUT=0; //继电器吸合
		     	    strcpy(ptr1_code,"OUT ON OK\x1a");

		     		 return(1);

		}
	 else if(strsearch1(i,"END",ptr1_code))
	   {


//		     		 IO_OUT=1;   // 继电器断开
//		     		strcpy(ptr1_code,"OUT OFF OK\x1a");
		     		 return(1);

		}




	//-------------------------------------------------------------------

		 else {

 		//短信号码删除 AT+CMGD=01
	err_dll:
					CMGD_GSM(ptr1_at,  sms_num_tmp );

					for(i=1;i<=MAX_TM-3;i++)
         			{    ptr1_at[i]=0; }

         		    Send_AT_Command(AT_CMGF1,ptr1_at,0);  //中文方式
				    Send_AT_Command(AT_CNMI,ptr1_at,0);//AT+CNMI=2,1

			  }



             return(0);





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

	    uchar idata ptr1[5],t;
     	t=0;
     	ptr1[t++]=(id/10)+0x30;
     	ptr1[t++]=(id%10)+0x30;
     	ptr1[t++]=0;ptr1[t++]=0x00;
	 Send_AT_Command(SMS_CMGR,ptr1_at,ptr1);
     timercount=0; while(timercount<20) ;

    // if(t=strsearch("+CMGR: 0,,0",ptr1_at))
	 if(receive_count<10)
	 {return(0);
	  }
      return(1);

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

     ptr1_code[0]=0;
	for(i=0;i<15;i++)
	{   //进行初始化,并显示  ATE0V0  = ATE0 ATV0   AT_E
		Send_AT_Command(AT_E,ptr1_at,0);
		Send_AT_Command(AT_INIT,ptr1_at,0);
		if(ptr1_at[1]==0x0d&&ptr1_at[0]==0x30)
		{  break;}
        else if(ptr1_at[receive_count-1]==0x0d&&ptr1_at[receive_count-2]=='0')
         {  break;}

		if(i==1)
		{  // IO_IGT=0;   for(i=0;i<=200;i++) dmsec(30);IO_IGT=1;
		    timercount=0; while(timercount<60);
		}


   	   timercount=0; while(timercount<60);

    }

	for(i=0;i<12;i++)
    	{            //读卡  AT+CPIN?

    		  Send_AT_Command(AT_E,ptr1_at,0);
			 Send_AT_Command(AT_INIT,ptr1_at,0);
    		 Send_AT_Command(AT_CPIN,ptr1_at,0) ;
    		 //如有收到回应进行分析收到的信息,
    		 timercount=0; while(timercount<2);
 			if(j=strsearch("READY",ptr1_at))
        			break;

            timercount=0; while(timercount<60);
    	}




 	Send_AT_Command(AT_IPR,ptr1_at,0);//波特率  AT+IPR=9600
  	Send_AT_Command(AT_CGMM,ptr1_at,0);  //模块型号 AT+CGMM


     timer_S_cnt=0; while(timer_S_cnt<12) ;
//     BELL=0;  for(i=0;i<=100;i++) dmsec(1);  BELL=1;
     Send_AT_Command(AT_CNMI,ptr1_at,0);
     Send_AT_Command(AT_CMGF1,ptr1_at,0);//AT+CMGF=1
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
//    IO_IGT=1;



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
//         GPS_ON=1;
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

     case AT_CMGF1:	//SMS 接收方式,
        strcpy(str_at,"AT+CMGF=1");
        break;
    // AT_CNMI
      case AT_CNMI:    //   AT+CNMI=2,1
          strcpy(str_at,"AT+CNMI=2,1");
        break;
       case  AT_IPR:   // 波特率 9600
        strcpy(str_at,"AT+IPR=9600"); break;

        case SMS_CMGL:		//读所有短信
         strcpy(uart_buff,"AT+CMGL=\"ALL\"");
       // strcpy(str_at,"AT+CMGL=4");
      //  strcat(uart_buff,ptr);
        break;

      case SMS_CMGR:		//读一个短信
        strcpy(str_at,"AT+CMGR=");
        strcat(str_at,str_code);
        break;

       case SMS_CMGD:		//删除一个短信
        strcpy(str_at,"AT+CMGD=");
        strcat(str_at,str_code);
        break;

       case AT_CPMS:
      	 strcpy(str_at,"AT+CPMS=\x22SM\x22");
        break;  //AT_CPMS

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
	uchar i;
	 Sys_Init();

	Initialize_Model(uart_buff,para_temp);//进行初始化
 

	receive_count=0;
	sms_tmp=0;



	while(1)
	{
		//-----------查询1到20条中的短信,只要有短信就处理------------------------------------------


				i=read_sms(uart_buff,para_temp,TEL_temp);           		//timer_S_cnt=0;
				if(i==0)
				{
				    receive_count=0;
		     		timer_S_cnt=0; while(timer_S_cnt<60)
		          	{
					   if(strsearch( "+CMTI:",uart_buff))
					     { break; }
					   if(receive_count>=7)
					      {break; }

		          	}


				}
               else
               {
                   //加入回复短信程序

                  	CMGD_GSM(uart_buff,sms_num_tmp );
               }





	}


}





























































