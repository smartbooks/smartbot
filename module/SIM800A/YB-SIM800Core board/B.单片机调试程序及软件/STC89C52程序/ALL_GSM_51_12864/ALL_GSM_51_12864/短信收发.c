/**************http://tpytongxin.taobao.com/****************/
#include <reg52.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit Send_SMS_PDU_key=P1^1;     //��PDU��ʽ���Ű���
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

    Ini_UART();    //��ʼ������
    AT();		   //��ʼ��ģ��
    delay(10);

    Print_Str(SMS_send);  //�������Ķ���
    delay(500);
    led(0x04);

    Print_Str(Sms2_Pdu);  //����������
    delay(500);
    led(0x08);

    Print_Char(0x1A);      //���ͽ�������
	delay(500);

	Print_Str("ATD18622944083;\r\n"); //��绰

    while(1);
}

void ser() interrupt 4
{



    if(RI==1)
    {  aa[j]=SBUF;//����浽��������

        RI=0; //�����������ж�
        j++;
    }


}
