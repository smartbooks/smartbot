/**************http://tpytongxin.taobao.com/****************/
#include <reg52.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit Send_SMS_PDU_key=P1^1;     //发PDU格式短信按键
sbit LED0=P0^0;
sbit LED1=P0^1;
sbit LED2=P0^2;
sbit LED3=P0^3;

#define MAXCHAR 81
uchar  aa[MAXCHAR];
code uchar ATE0[]="ATE0\r\n";
code uchar CREG_CMD[]="AT+CREG?\r\n";
code uchar SMS_send[]="AT+CMGS=18\r\n";
code uchar ATCN[]="AT+CNMI=2,1\r\n";
code uchar CMGF[]="AT+CMGF=0\r\n";
code uchar CMGR[12]="AT+CMGR=1\r\n";
code uchar CMGD[12]="AT+CMGD=1\r\n";

/*-----------------------------------------------------------------------
1.如果你的晶振是11.0592M
只需要修改下面的号码就可以了,给成你手上拿着的手机的号码


修改方法   在下面这段字符中找到 5129021411F5 
 
 其实5129021411F5 --> 15922041115
18622044083 8126924480F3


 看明白了吗  电话是两位两位颠倒 将您手上的手机号码替换即可
-----------------------------------------------------------------------*/
uchar  code Sms2_Pdu[]="0891683108200205F011000B818126924480F30008A704521B601D";
/*-----------------------------------------------------------------------
1.如果你的晶振不是11.0592M

则修改 Ini_UART 函数 波特率 9600


其他都不用动

-----------------------------------------------------------------------*/

uchar a,j=0,flag=0;

void delay(uint ms)// 延时子程序
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
        delay(2);
        str++;
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
    TMOD |= 0x20 ; //TMOD: timer 1, mode 2, 8-bit reload
    //定时器1为模式2,8位自动重装
    PCON |= 0x80 ; //SMOD=1;
    TH1 = 0xFA ;   //Baud:19200 fosc="11".0592MHz
    TL1=0xFA;
    IE |= 0x90 ;     //Enable Serial Interrupt
    TR1 = 1 ;       // timer 1 run
    TI=1;
    ES=1;
}

void clearBuff(void)
{
    for(j=0;j<MAXCHAR;j++)
    {
        aa[j]=0x00;
    }
    j=0;
}

void led(int i)
{
    P2 |= i;
    delay(20);
    P2 &= ~i;
    delay(20);
    P2 |= i;
    delay(20);
    P2 &= ~i;
}



void AT(void)
{

    while(1)
    {
        Print_Str(ATE0);
        delay(50);

        Print_Str(ATCN);
        delay(50);

        Print_Str(CMGF);
        delay(100);

        clearBuff();
        Print_Str(CREG_CMD);
        delay(50);


        if(((aa[9]=='0')&&(aa[11]=='1'))||((aa[9]=='0')&&(aa[11]=='5')))
        {
            clearBuff();
            led(0x02);
            break;
        }
        else
        {
            clearBuff();

            led(0x01);
            delay(50);
        }
    }
}


void main()
{

    Ini_UART();    //初始化串口
    AT();		   //初始化模块
    delay(10);

    Print_Str(SMS_send);  //发送中文短信
    delay(500);
    led(0x04);

    Print_Str(Sms2_Pdu);  //发短信内容
    delay(500);
    led(0x08);

    Print_Char(0x1A);      //发送结束符号
	delay(500);

	Print_Str("ATD18622944083;\r\n"); //打电话

    while(1);
}

void ser() interrupt 4
{



    if(RI==1)
    {  aa[j]=SBUF;//命令存到命令数组

        RI=0; //软件清除接收中断
        j++;
    }


}
