
#ifndef __DS1302_H__
#define __DS1302_H__

//����MCU�ͺ�89��15
#define mcu 15

//��������
#if (mcu==15)
//STC15ϵ��ͷ�ļ�
#include <STC15.H>

#elif (mcu==89)

//STC89ϵ��ͷ�ļ�
#include <reg52.h>
#endif

//STC89-DS1302���Ž���
//sbit SCK=P0^2;    //SCLK
//sbit SDA=P0^1;    //DATA
//sbit RST=P0^0;	//REST

//STC15-DS1302���Ž���
sbit SCK = P0^2; //SCLK
sbit SDA = P0^1; //DATA
sbit RST = P0^0; //REST

//��λ��
#define RST_CLR	RST=0//��ƽ�õ�
#define RST_SET	RST=1//��ƽ�ø�

//˫������
#define IO_CLR	SDA=0//��ƽ�õ�
#define IO_SET	SDA=1//��ƽ�ø�
#define IO_R	SDA  //��ƽ��ȡ

//ʱ���ź�
#define SCK_CLR	SCK=0//ʱ���ź�
#define SCK_SET	SCK=1//��ƽ�ø�

#define ds1302_sec_add			0x80		//�����ݵ�ַ
#define ds1302_min_add			0x82		//�����ݵ�ַ
#define ds1302_hr_add			0x84		//ʱ���ݵ�ַ
#define ds1302_date_add			0x86		//�����ݵ�ַ
#define ds1302_month_add		0x88		//�����ݵ�ַ
#define ds1302_day_add			0x8a		//�������ݵ�ַ
#define ds1302_year_add			0x8c		//�����ݵ�ַ
#define ds1302_control_add		0x8e		//�������ݵ�ַ
#define ds1302_charger_add		0x90
#define ds1302_clkburst_add		0xbe

extern unsigned char time_buf1[8];//��������ʱ������

extern unsigned char time_buf[8] ;//��������ʱ������

/*------------------------------------------------
           ��DS1302д��һ�ֽ�����
------------------------------------------------*/
void Ds1302_Write_Byte(unsigned char addr, unsigned char d);

/*------------------------------------------------
           ��DS1302����һ�ֽ�����
------------------------------------------------*/
unsigned char Ds1302_Read_Byte(unsigned char addr) ;

/*------------------------------------------------
           ��DS1302д��ʱ������
------------------------------------------------*/
void Ds1302_Write_Time(void) ;

/*------------------------------------------------
           ��DS1302����ʱ������
------------------------------------------------*/
void Ds1302_Read_Time(void);


/*------------------------------------------------
                DS1302��ʼ��
------------------------------------------------*/
void Ds1302_Init(void);

#endif
