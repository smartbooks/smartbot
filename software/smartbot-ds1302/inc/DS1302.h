
#ifndef __DS1302_H__
#define __DS1302_H__

//设置MCU型号89、15
#define mcu 15

//条件编译
#if (mcu==15)
//STC15系列头文件
#include <STC15.H>

#elif (mcu==89)

//STC89系列头文件
#include <reg52.h>
#endif

//STC89-DS1302引脚接线
//sbit SCK=P0^2;    //SCLK
//sbit SDA=P0^1;    //DATA
//sbit RST=P0^0;	//REST

//STC15-DS1302引脚接线
sbit SCK = P0^2; //SCLK
sbit SDA = P0^1; //DATA
sbit RST = P0^0; //REST

//复位脚
#define RST_CLR	RST=0//电平置低
#define RST_SET	RST=1//电平置高

//双向数据
#define IO_CLR	SDA=0//电平置低
#define IO_SET	SDA=1//电平置高
#define IO_R	SDA  //电平读取

//时钟信号
#define SCK_CLR	SCK=0//时钟信号
#define SCK_SET	SCK=1//电平置高

#define ds1302_sec_add			0x80		//秒数据地址
#define ds1302_min_add			0x82		//分数据地址
#define ds1302_hr_add			0x84		//时数据地址
#define ds1302_date_add			0x86		//日数据地址
#define ds1302_month_add		0x88		//月数据地址
#define ds1302_day_add			0x8a		//星期数据地址
#define ds1302_year_add			0x8c		//年数据地址
#define ds1302_control_add		0x8e		//控制数据地址
#define ds1302_charger_add		0x90
#define ds1302_clkburst_add		0xbe

extern unsigned char time_buf1[8];//空年月日时分秒周

extern unsigned char time_buf[8] ;//空年月日时分秒周

/*------------------------------------------------
           向DS1302写入一字节数据
------------------------------------------------*/
void Ds1302_Write_Byte(unsigned char addr, unsigned char d);

/*------------------------------------------------
           从DS1302读出一字节数据
------------------------------------------------*/
unsigned char Ds1302_Read_Byte(unsigned char addr) ;

/*------------------------------------------------
           向DS1302写入时钟数据
------------------------------------------------*/
void Ds1302_Write_Time(void) ;

/*------------------------------------------------
           从DS1302读出时钟数据
------------------------------------------------*/
void Ds1302_Read_Time(void);


/*------------------------------------------------
                DS1302初始化
------------------------------------------------*/
void Ds1302_Init(void);

#endif
