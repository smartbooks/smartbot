#include <reg52.h>
#include <string.h>
sbit p2_0=P2^0;  //用于单片机的串口收到模块信息时，用LED显示
sbit p1_4=P1^4;  //用于单片机的串口收到模块信息时，确定收到信息的内容
sbit P0_0=P0^0;
sbit P0_1=P0^1;
unsigned char ctrlz=0x1a;//短信息结束符ctrl＋z
unsigned char ATH[]="ATH\r";  //挂断电话指令
unsigned char ATD[]="ATD02228214113;\r";  //打电话指令
unsigned char CMGF_1[]="AT+CMGF=1\r";   //设置短消息为英文方式
unsigned char CSCA[]="AT+CSCA=\"+8613010761500\"\r";  //设置短信中心号码
unsigned char CMGS[]="AT+CMGS=\"13462405067\"\r";       //拨打的电话号码

 
unsigned char str[20];
 unsigned char count=0;
 unsigned char flag=0;
 //unsigned char choice=0;

void Delay_xMs(unsigned long int x)
{
    unsigned int i,j;
    for( i =0;i < x;i++ )
    {
        for( j =0;j<3;j++ );
    }
}
void send_modem_string(unsigned char *modem_string)//指令字符串发送指令
{
  while(*modem_string)
      {
	    SBUF = *modem_string;
	    while(TI==0);
		TI=0;
	    modem_string++;
	  }
}

void send_SMS_english()//发送英文短信息函数，P0。1为低电平时有效，即执行该函数
{
   send_modem_string(CMGF_1); //发送AT+CMGF=1指令,短信为英文
   Delay_xMs(1000);
   send_modem_string(CSCA);//发送AT+CSCA="+8613010761500"指令，设置短信息中心号码
   Delay_xMs(1000);
   send_modem_string(CMGS); //发送AT+CMGS=15939187275指令，设置短信息发送到的手机号码
   Delay_xMs(1000);
   send_modem_string("The highest temperature is");//发送短信息内容，但没有ctrl＋z结束符
   send_modem_string("13 ^C");
//   SBUF=0x5F;//
//   while(TI==0);
 //  TI=0;
 //  SBUF=0x53;//
 //  while(TI==0);
 //  TI=0;

   SBUF=ctrlz;//发送短信息结束符ctrl＋z,这是必须的。
   while(TI==0);
   TI=0;
}


void serial_recieve(unsigned char a)
{
    while(RI==1)
		{
	           RI=0;
		   str[count]=SBUF;
		   count++;
  		   while((RI==0)&(count<a));//单片机接受数据的缓冲时间，如果没有时间缓冲，则无法正常接受模块发送给单片机的反馈信息。
		}
	count=0;

    //Delay_xMs(1000);


//	return 1;
}

void main()
{
  ///////功能:串口初始化,波特率115200(9600)，方式1///////
  TMOD = 0x20;
  PCON = 0x00;
  SCON = 0x50;
  TH1 = 0xf3;
  TL1 = 0xf3;
  TR1 = 1;
  // EA=1;     //开CPU中断和串行口中断
 // ES=1;


  Delay_xMs(1000);

  send_modem_string(ATD);   //打电话


  while(1)
   {
      while(RI==1)  //一旦收到模块的返回信息，点亮P2.0端口的LED 1000ms后熄灭
	     {
		  p2_0=0;
		  Delay_xMs(1000);
		  p2_0=1;
                  RI = 0;
		 }
	  //P0_0=0;
      if(P0_0==0)
        {
             send_modem_string(ATH);//断开电话通话连接
	     serial_recieve(8);//函数参数应该（大于等于（发送字符数（3）＋5））且小于等于（（发送字符数（3）＋7））
         if((str[6]=='O')&(str[7]=='K'))
		   {              //此处添加断开电话成功时，屏幕上显示'断开成功'的信息
		    p1_4=0;
		    Delay_xMs(1000);
		    p1_4=1;
	       }
		 else
		   {
		        //此处断开电话不成功时的信息
		   }
		 RI=0;
         P0_0=1;
        }
     if(P0_1==0)
     {
       send_SMS_english();

       P0_1=1;
     }

   }
}

