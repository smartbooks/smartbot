/********************************************************
����������GSMģ�������ɹ���������Ƭ������Ƭ���������Ρ���
�󣬿�ʼ����GSMģ�鷢��һ�����Ķ��ţ����̵��ӻ�ӭ����123��
��������Ϻ󡣵�Ƭ���������Ρ��������̽�����
********************************************************/
#include<reg52.h>
#include<intrins.h>
#include<stdlib.h>
#define uchar unsigned char
#define uint unsigned int
sbit FMQ=P3^6;	//������ 
uchar code at[]="AT\r";
uchar code TC_MSXZ[]={"AT+CMGF=0\r"};//	����ģʽѡ��ΪPDU�����Է��������ַ�
uchar code TC_MDSJ[]={"AT+CMGS=37\r"}; // ��Ϣ����
uchar code TC_FSNR[]={"0891683108200905F011000D91685191418135F80008AA169F997A0B75355B506B228FCE60A80021003100320033"};

uchar num,temp,key,m=3,n=0,z=0;
void delay_1ms(uint z) //��ʱ����
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}
/****************  ���ݷ��� *****************/
 void send(uchar *tab)
 {
 	while((*tab)!='\0')
	{
	//	ES=0; //�ش����ж�
		SBUF=*tab;
		while(TI==0);
		TI=0;
		tab++;
	//	ES=1;  //�������ж�
	}
 }
/********** ���ڳ�ʼ�� ***********/
 void uart_int(void)
 {
 	SCON=0x50;//UART��ʽ1��8λUART��
	REN =1;	  //�����пڽ������ݣ�
	PCON=0x00;//SMOD=0�������ʲ��ӱ���
	TMOD=0x20;//T1��ʽ2������UART�����ʣ�
	TH1 =0xFD; //װ��ֵ��
	TL1 =0xFD; //װ��ֵ
	TR1 =1;	   //������ʱ��1
//	EA=1;   //��ȫ���жϿ���
//	ES=1;    //�򿪴��п��ж�
 }
void fengmingqi() //������
{	uint b,r;
	for (b=0;b<300;b++)
	{
		FMQ=0;
		for(r=50;r>0;r--);
		FMQ=1;
		for(r=50;r>0;r--);	
	}	
}
void main()
{
/*	DLED=1;
	P0=0x00;
	DLED=0;
	n=0;
	init();	*/
	delay_1ms(1000);
	fengmingqi();  //�������Ρ���
	uart_int();//���ڳ�ʼ��
	send(at);  //����AT
	delay_1ms(500);
	
	
	send(TC_MSXZ); //����ģʽѡ��
	delay_1ms(500);
	send(TC_MDSJ); //��Ϣ����
	delay_1ms(500);
	send(TC_FSNR); //��������
	delay_1ms(500);
	SBUF=0x1A;	  //16���Ʒ���1A
	
	
	while(TI==0);
	TI=0;
	fengmingqi(); //�������Ρ���
	delay_1ms(500);
	while(1);	  //�������
}