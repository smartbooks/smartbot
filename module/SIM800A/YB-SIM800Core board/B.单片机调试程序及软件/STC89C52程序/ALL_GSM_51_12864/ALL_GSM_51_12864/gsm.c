/**************http://tpytongxin.taobao.com/****************/
#include <reg52.h>
#include <intrins.h>
#include <string.h>
#include <stdio.h>
#include "lcd12864.h"

#define uint unsigned int
#define uchar unsigned char

sbit Send_ENGLISH_key=P3^2;     //��Ӣ��TEXT���Ű���
sbit Send_CALLING_key=P3^3;     //��绰����
sbit Send_DS18B20_key=P3^4;     //���¶Ȱ���
sbit Send_SMS_PDU_key=P3^5;     //������PDU���Ű���

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

#define CALL_YOU_TELNUMBER "ATD18622944083;\r\n"    //���������ϵ��ֻ����� ���ǰ��ӵ�
#define SEND_MESSA_TO_YOUR "at+cmgs=\"18622944083\"\r\n"    //���������ϵ��ֻ����� ���ǰ��ӵ�
#define SEND_MESSA_CONTENT "tpytongxin.taobao.com"
#define CALL_YOU_TELNUMBER "ATD18622944083;\r\n"    //���������ϵ��ֻ����� ���ǰ��ӵ�
#define CALL_YOU_TELNUMBER "ATD18622944083;\r\n"    //���������ϵ��ֻ����� ���ǰ��ӵ�

sbit JIDIANQI=P1^0;

sbit FENGMINGQI=P1^1;



/*-----------------------------------------------------------------------
1.�����ľ�����11.0592M
ֻ��Ҫ�޸�����ĺ���Ϳ�����,�������������ŵ��ֻ��ĺ���


�޸ķ���   ����������ַ����ҵ� 5129021411F5

 ��ʵ5129021411F5 --> 15922041115
18622044083 8126924480F3


 ����������  �绰����λ��λ�ߵ� �������ϵ��ֻ������滻����
-----------------------------------------------------------------------*/
uchar  code Sms2_Pdu[]="0891683108200205F011000B818126924480F30008A704521B601D";
/*-----------------------------------------------------------------------
1.�����ľ�����11.0592M

���޸� Ini_UART ���� ������ 9600


���������ö�

-----------------------------------------------------------------------*/

uchar a,j=0,flag=0;
int g_timeout=0;

void delay(uint ms)// ��ʱ�ӳ���
{
    uchar i;
    while(ms--)
    {
        for(i=0;i<120;i++);
    }
}


/***********************************************************
�������ƣ�Print_Char
�������ܣ����͵����ַ�
          ��ڲ���:ch      ���ڲ�������
***********************************************************/
Print_Char(uchar ch)//���͵����ַ�
{
    SBUF=ch; //���뻺����
    while(TI!=1); //�ȴ��������
    TI=0; //�������
}

/***********************************************************
�������ƣ�Print_Str
�������ܣ������ַ���
          ��ڲ���:*str    ���ڲ�������
***********************************************************/
Print_Str(uchar *str)//�����ַ���
{

    while(*str!='\0')
    {
        Print_Char(*str);
        delay(2);
        str++;
    }

}

/***********************************************************
�������ƣ�Ini_UART
�������ܣ����ڳ�ʼ������ʱ����ʼ��
          ��ڲ���:��     ���ڲ�������
***********************************************************/
Ini_UART(void)//���ڳ�ʼ������ʱ����ʼ��
{
    SCON = 0x50 ;  //SCON: serail mode 1, 8-bit UART, enable ucvr
    //UARTΪģʽ1��8λ���ݣ��������
    TMOD |= 0x20 ; //TMOD: timer 1, mode 2, 8-bit reload
    //��ʱ��1Ϊģʽ2,8λ�Զ���װ
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
    if(strstr(aa,"OK")!=NULL)             //��ⵥƬ����ģ�������
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
        v_Lcd12864PutString_f( 1,1, "ģ�����ӡ�..��") ;
        clearBuff();
        Print_Str(ATE0);
    }
    led(0x02);

    v_Lcd12864PutString_f( 1,1, "ģ�����ӡ�OK��") ;

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
            v_Lcd12864PutString_f( 1,1, "ģ��ע�᡾OK��") ;
            led(0x08);
            break;
        }
        else
        {
            clearBuff();
            v_Lcd12864PutString_f( 1,1, "ģ��ע�᡾..��") ;
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

    Print_Str(SEND_MESSA_CONTENT);  //����������
    delay(10);
    Print_Char(0x1A);      //���ͽ�������

    delay(10);
    led(0x04);
    clearBuff();
}



void send_call()
{
    clearBuff();
    delay(100);
    Print_Str(CALL_YOU_TELNUMBER); //��绰
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

    Print_Str(SMS_send);  //�������Ķ���

    delay(500);
    led(0x02);

    Print_Str(Sms2_Pdu);  //����������
    Print_Char(0x1A);      //���ͽ�������

    delay(500);
    led(0x04);
    clearBuff();
}


void main()
{

    v_Lcd12864Init_f() ;
    v_Lcd12864PutString_f( 2,0, "") ;
    v_Lcd12864PutString_f( 2,1, "��˼ͨ��") ;
    v_Lcd12864PutString_f( 1,2, "") ;
    v_Lcd12864PutString_f( 1,3, "www.csgsm.com") ;

    delay(500);
    Ini_UART();             //��ʼ������
    v_Lcd12864PutString_f( 1,1, "����������OK��") ;

    AT();		   //��ʼ��ģ��
    delay(10);


    Send_ENGLISH_key=1;     //��Ӣ��TEXT���Ű���
    Send_CALLING_key=1;     //��绰����
    Send_DS18B20_key=1;     //���¶Ȱ���
    Send_SMS_PDU_key=1;     //������PDU���Ű���

    clearBuff();
    Print_Str(CMGD);
    delay(50);
    while(Hand()==0);
    clearBuff();

        Print_Str(CMGF1);
        delay(30);

        Print_Str(ATCN);
        delay(30);



    v_Lcd12864PutString_f( 1,1, "����绰��..��") ;
    send_call();
    v_Lcd12864PutString_f( 1,1, "����绰��OK��") ;

    while(1)
    {
        ////////////////////////////////////////////////////////////////////////
        if(Send_ENGLISH_key==0)     //��Ӣ��TEXT���Ű���
        {
            v_Lcd12864PutString_f( 1,1, "Ӣ�Ķ��š�..��") ;
            send_english();
            v_Lcd12864PutString_f( 1,1, "Ӣ�Ķ��š�OK��") ;

        }
        else if(Send_CALLING_key==0)     //��绰����
        {
            v_Lcd12864PutString_f( 1,1, "����绰��..��") ;
            send_call();
            v_Lcd12864PutString_f( 1,1, "����绰��OK��") ;

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
            v_Lcd12864PutString_f( 1,1, "�绰���ˡ��ס�") ;
            clearBuff();
        }
        else
        {
            FENGMINGQI=1;
        }



        if(strstr(aa,"+CMTI")!=NULL)    //��������+CMTI�ַ�
        {
            clearBuff();
            Print_Str(CMGF1);
            delay(50);
            while(Hand()==0);
            clearBuff();

            Print_Str(CMGR);            //��ȡ1��λ�õĶ���
            delay(100);


            if(strstr(aa,"k1")!=NULL)
            {
                v_Lcd12864PutString_f( 1,1, "���̵�����OK��") ;
                JIDIANQI=0;
            }
            else if(strstr(aa,"k0")!=NULL)
            {
                v_Lcd12864PutString_f( 1,1, "�ؼ̵�����OK��") ;
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
    {  aa[j]=SBUF;//����浽��������

        RI=0; //�����������ж�
        j++;
    }


}
