/****1602Һ����������******���µ�������*******************
**********************************************************
****�ɶ�������4���������ԸĶ�
****
****������ʾ������ĸ������
****
********************************************************/
#include<reg52.h>
#include <intrins.h>
#include <absacc.h>
#define uchar unsigned char 					//����unsigned char Ϊ uchar
#define uint  unsigned int
#define BUSY  0x80                               //lcdæ����־
//-----------�ɶ�����--------------------------
#define DATAPORT P0                             //����P0��ΪLCDͨѶ�˿�
sbit LCM_RS=P3^2;								 //����/�����
sbit LCM_RW=P3^3;								 //��/дѡ���
sbit LCM_EN=P3^4;
//----------------------------------------------
void initLCM( void);               											//LCD��ʼ���ӳ���
void lcd_wait(void);              											//LCD���æ�ӳ���
void WriteCommandLCM(uchar WCLCM,uchar BusyC);                              //дָ�ICM�Ӻ���
void WriteDataLCM(uchar WDLCM);                                             //д���ݵ�LCM�Ӻ���
void DisplayOneChar(uchar X,uchar Y,uchar DData);                           //��ʾָ�������һ���ַ��Ӻ���
void DisplayListChar(uchar X,uchar Y,uchar code *DData);                    //��ʾָ�������һ���ַ��Ӻ���
/***************һ΢����ʱ�ӳ���******************/
void delayus(uint delay){
	while(delay--);
}
/***************һ������ʱ�ӳ���******************/
void delay_LCM(uint delay){
	while(delay--)
		delayus(149);
}
/**********дָ�LCM�Ӻ���************/
void WriteCommandLCM(uchar WCLCM,uchar BusyC)
{
    if(BusyC)lcd_wait();
	DATAPORT=WCLCM;
    LCM_RS=0;                   // ѡ��ָ��Ĵ���
    LCM_RW=0;               	// дģʽ	
    LCM_EN=1;
	_nop_();
	_nop_();
	_nop_();
    LCM_EN=0;
}
/**********д���ݵ�LCM�Ӻ���************/
void WriteDataLCM(uchar WDLCM)
{
    lcd_wait( );                //���æ�ź�
	DATAPORT=WDLCM;
    LCM_RS=1;                   // ѡ�����ݼĴ���
    LCM_RW=0;           	    // дģʽ
    LCM_EN=1;
    _nop_();
	_nop_();
	_nop_();
    LCM_EN=0;
}
/***********lcm�ڲ��ȴ�����*************/
void lcd_wait(void)
{
    DATAPORT=0xff;	
	LCM_EN=1;
    LCM_RS=0;   			
    LCM_RW=1;   			
    _nop_();
    while(DATAPORT&BUSY)
	{  LCM_EN=0;
	   _nop_();
	   _nop_();
	   LCM_EN=1;
	   _nop_();
	   _nop_();
	   }
   	LCM_EN=0;
	
}
/**********LCM��ʼ���Ӻ���***********/
void initLCM( )
{
	DATAPORT=0;	
	delay_LCM(15);
	WriteCommandLCM(0x38,0);    //������ʾģʽ���ã������æ�ź�
    delay_LCM(5);
    WriteCommandLCM(0x38,0);
    delay_LCM(5);
    WriteCommandLCM(0x38,0);
    delay_LCM(5);

    WriteCommandLCM(0x38,1);    //8bit���ݴ��ͣ�2����ʾ��5*7���ͣ����æ�ź�
    WriteCommandLCM(0x08,1);    //�ر���ʾ�����æ�ź�
    WriteCommandLCM(0x01,1);    //���������æ�ź�
    WriteCommandLCM(0x06,1);    //��ʾ����������ã����æ�ź�
    WriteCommandLCM(0x0c,1);    //��ʾ���򿪣���겻��ʾ������˸�����æ�ź�
}
/****************��ʾָ�������һ���ַ��Ӻ���*************/
void DisplayOneChar(uchar X,uchar Y,uchar DData)
{
    Y&=1;
    X&=15;
    if(Y)X|=0x40;               //��yΪ1����ʾ�ڶ��У�����ַ��+0X40
    X|=0x80;                    //ָ����Ϊ��ַ��+0X80
    WriteCommandLCM(X,0);
    WriteDataLCM(DData);
}
/***********��ʾָ�������һ���ַ��Ӻ���***********/
void DisplayListChar(uchar X,uchar Y,uchar *DData)
{
    uchar ListLength=0;
    Y&=0x01;
    X&=0x0f;
    while(X<16)
    {
        DisplayOneChar(X,Y,DData[ListLength]);
        ListLength++;
        X++;
    }
}
/**************************������********************************************/
void LCD(uchar Frist_line[],uchar Second_line[])
{
EA=0;  //����������Ҫ���ж�

  //P1=0xff;                                   				  //��ʼ��p1�ڣ�ȫ��Ϊ1
	delay_LCM(50);	                                          //��ʱ500ms����
	initLCM( );                               			  	  //LCD��ʼ��
	DisplayListChar(0,0,Frist_line);	
	DisplayListChar(0,1,Second_line);
EA=1;//���ж�
}
void LCD1(uchar Frist_line[])
{
EA=0;  //����������Ҫ���ж�

  //P1=0xff;                                   				  //��ʼ��p1�ڣ�ȫ��Ϊ1
	delay_LCM(50);	                                          //��ʱ500ms����
	initLCM( );                               			  	  //LCD��ʼ��
	DisplayListChar(0,0,Frist_line);	
//	DisplayListChar(0,1,Second_line);
EA=1;//���ж�
}
