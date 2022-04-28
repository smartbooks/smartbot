/**************http://tpytongxin.taobao.com/****************/
#include <reg52.h>
#include <intrins.h>
#include <string.h>
#include <stdio.h>
#include "lcd12864.h"

#define uint unsigned int
#define uchar unsigned char

sbit Send_ENGLISH_key=P3^2;     //发英文TEXT短信按键
sbit Send_CALLING_key=P3^3;     //打电话按键
sbit Send_DS18B20_key=P3^4;     //发温度按键
sbit Send_SMS_PDU_key=P3^5;     //发中文PDU短信按键

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
code uchar CMGF0[]="AT+CMGF=0\r\n";
code uchar CMGF1[]="AT+CMGF=1\r\n";
code uchar CMGR[12]="AT+CMGR=1\r\n";
code uchar CMGD[12]="AT+CMGD=1\r\n";

#define CALL_YOU_TELNUMBER "ATD18622944083;\r\n"    //填入你手上的手机号码 不是板子的
#define SEND_MESSA_TO_YOUR "at+cmgs=\"18622944083\"\r\n"    //填入你手上的手机号码 不是板子的
#define SEND_MESSA_CONTENT "tpytongxin.taobao.com"
#define CALL_YOU_TELNUMBER "ATD18622944083;\r\n"    //填入你手上的手机号码 不是板子的
#define CALL_YOU_TELNUMBER "ATD18622944083;\r\n"    //填入你手上的手机号码 不是板子的

sbit JIDIANQI=P1^0;

sbit FENGMINGQI=P1^1;



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
int g_timeout=0;

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

int Hand()
{
    delay(10);
    if(strstr(aa,"OK")!=NULL)             //检测单片机和模块的连接
    {
        g_timeout=0;
        return 1;
    }
    if(g_timeout>50)
    {
        g_timeout=0;
        return -1;
    }
    g_timeout++;
    return 0;
}



void AT(void)
{


    clearBuff();
    Print_Str(ATE0);
    delay(50);


    while(strstr(aa,"OK")==NULL)
    {
        delay(50);
        led(0x01);
        v_Lcd12864PutString_f( 1,1, "模块连接【..】") ;
        clearBuff();
        Print_Str(ATE0);
    }
    led(0x02);

    v_Lcd12864PutString_f( 1,1, "模块连接【OK】") ;

    Print_Str(ATCN);
    delay(50);


    while(1)
    {
        clearBuff();
        Print_Str(CREG_CMD);
        delay(50);
        if(((aa[9]=='0')&&(aa[11]=='1'))||((aa[9]=='0')&&(aa[11]=='5')))
        {
            clearBuff();
            v_Lcd12864PutString_f( 1,1, "模块注册【OK】") ;
            led(0x08);
            break;
        }
        else
        {
            clearBuff();
            v_Lcd12864PutString_f( 1,1, "模块注册【..】") ;
            led(0x04);
            delay(50);
        }
    }
}

void send_english()
{
    P2=0XFF;
    clearBuff();

    Print_Str(CMGF1);
    delay(100);
    led(0x01);


    Print_Str(SEND_MESSA_TO_YOUR);

    delay(300);
    led(0x02);

    Print_Str(SEND_MESSA_CONTENT);  //发短信内容
    delay(10);
    Print_Char(0x1A);      //发送结束符号

    delay(10);
    led(0x04);
    clearBuff();
}



void send_call()
{
    clearBuff();
    delay(100);
    Print_Str(CALL_YOU_TELNUMBER); //打电话
    clearBuff();
    delay(300);
}

void send_18B20()
{

}

void send_pdu()
{
    clearBuff();
    Print_Str(CMGF0);
    delay(100);
    led(0x01);

    Print_Str(SMS_send);  //发送中文短信

    delay(500);
    led(0x02);

    Print_Str(Sms2_Pdu);  //发短信内容
    Print_Char(0x1A);      //发送结束符号

    delay(500);
    led(0x04);
    clearBuff();
}


void main()
{

    v_Lcd12864Init_f() ;
    v_Lcd12864PutString_f( 2,0, "") ;
    v_Lcd12864PutString_f( 2,1, "创思通信") ;
    v_Lcd12864PutString_f( 1,2, "") ;
    v_Lcd12864PutString_f( 1,3, "www.csgsm.com") ;

    delay(500);
    Ini_UART();             //初始化串口
    v_Lcd12864PutString_f( 1,1, "串口启动【OK】") ;

    AT();		   //初始化模块
    delay(10);


    Send_ENGLISH_key=1;     //发英文TEXT短信按键
    Send_CALLING_key=1;     //打电话按键
    Send_DS18B20_key=1;     //发温度按键
    Send_SMS_PDU_key=1;     //发中文PDU短信按键

    clearBuff();
    Print_Str(CMGD);
    delay(50);
    while(Hand()==0);
    clearBuff();

        Print_Str(CMGF1);
        delay(30);

        Print_Str(ATCN);
        delay(30);



    v_Lcd12864PutString_f( 1,1, "给你电话【..】") ;
    send_call();
    v_Lcd12864PutString_f( 1,1, "给你电话【OK】") ;

    while(1)
    {
        ////////////////////////////////////////////////////////////////////////
        if(Send_ENGLISH_key==0)     //发英文TEXT短信按键
        {
            v_Lcd12864PutString_f( 1,1, "英文短信【..】") ;
            send_english();
            v_Lcd12864PutString_f( 1,1, "英文短信【OK】") ;

        }
        else if(Send_CALLING_key==0)     //打电话按键
        {
            v_Lcd12864PutString_f( 1,1, "给你电话【..】") ;
            send_call();
            v_Lcd12864PutString_f( 1,1, "给你电话【OK】") ;

        }
        else if(Send_SMS_PDU_key==0)
        {

        }

        ////////////////////////////////////////////////////////////////////////

        //        +CMTI: "SM",1


        if(strstr(aa,"RING")!=NULL)
        {
            FENGMINGQI=0;
            delay(2);
            v_Lcd12864PutString_f( 1,1, "电话来了【亲】") ;
            clearBuff();
        }
        else
        {
            FENGMINGQI=1;
        }



        if(strstr(aa,"+CMTI")!=NULL)    //缓存中有+CMTI字符
        {
            clearBuff();
            Print_Str(CMGF1);
            delay(50);
            while(Hand()==0);
            clearBuff();

            Print_Str(CMGR);            //读取1号位置的短信
            delay(100);


            if(strstr(aa,"k1")!=NULL)
            {
                v_Lcd12864PutString_f( 1,1, "开继电器【OK】") ;
                JIDIANQI=0;
            }
            else if(strstr(aa,"k0")!=NULL)
            {
                v_Lcd12864PutString_f( 1,1, "关继电器【OK】") ;
                JIDIANQI=1;
            }
            else
            {
                v_Lcd12864PutString_f( 0,3, strstr(aa,",,")) ;
            }

            clearBuff();

            Print_Str(CMGD);
            delay(50);
            while(Hand()==0);
            clearBuff();
        }
        ////////////////////////////////////////////////////////////////////////
    }
}

void ser() interrupt 4
{

    if(RI==1)
    {  aa[j]=SBUF;//命令存到命令数组

        RI=0; //软件清除接收中断
        j++;
    }


}
