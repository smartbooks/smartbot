#pragma OPTIMIZE(6,size)   	//优化级别与优化方式
//#pragma LARGE 				//编译模式 SMALL  COMPACT LARGE


#include <ctype.h>			//字符函数
#include <stdio.h>		//一般IO/函数
#include <string.h>			//字符串函数
//#include <stdlib.h>			//标准函数
//#include <mach.h>			//数学函数
#include <intrins.h>		//内部函数
//#include <stdarg.h>		//变量参数表


typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;


//#include <struct.h>
//#include <EventDef.h>
#define  CPU_TYPE2   1    //定义CPU的类型
#define  W78E58      1	 //可适于通用8位单片机 如AT89S52（S51肯定不行）
#define  STC89E58    2
//--------标准51单片机－－－新茂产品－－－－－－－－－－－－－
#include <CPU.h>

#if   CPU_TYPE2==STC89E58

    sfr  S2CON    = 0x9A;
	//S2SM0,S2SM1,S2SM2,S2REN,S2TB8,SRB8,S2TI,S2RI
	sfr  IE2    = 0xAF;
	//X,X,X,X,X,X,ESPI,ES2
	sfr  S2BUF    = 0x9B;
	sfr  AUXR    = 0x8e;
	sfr	 BRT	=	0x9c;//独立波特率发生器定时器，重装值
	sfr  IAP_CONTR =  0xC7;
	sfr  CCON    = 0xD8;
	sfr  CMOD    = 0xD9;
	sfr  CL      = 0xE9;
	sfr  CH      = 0xF9;
	sfr  CCAP0L  = 0xEA;
	sfr  CCAP0H  = 0xFA;
	sfr  CCAPM0  = 0xDA;
	sfr  CCAPM1  = 0xDB;
	sbit CR      = 0xDE;

#endif
//-----------------------------------------------------------------------------------------
	#define system_sleep   		_nop_()
	#define system_powerdown	_nop_()
	#define watch_dog_clear		_nop_()

 	#define _Nop()    _nop_()
 	#define BELL  P3_3
 	#define IO_OUT			P3_6
	#define GPS_ON			P1_1 //GPS开关
	sbit IO_IGT 	=P3^4;
	//外部中断
	sbit IO_P35		=P3^2;
	sbit IO_P24		=P2^4;
	sbit IO_P25		=P2^5;
    sbit IO_P27		=P2^7;

    sbit LED_S0 = P0^0;
    sbit LED_S1 = P0^6;


//=============================================================================
//                  基本的常用常量
//=============================================================================
#define ON		1
#define OFF		0
#define TRUE	1
#define true    1
#define FALSE	0



//=============================================================================
//             定义 SIM300 的命令列表
//=============================================================================

#define AT_INIT 			1	//回复状态
#define AT_E   				2  //回显方式 
#define AT_CNMI   			4	//
#define AT_CGMM    			5	//
#define SMS_CMGL			6  //读所有短信
#define  AT_CPIN			7
#define AT_COMMAND          8
#define SMS_CMGR			9   //读一个短信
#define SMS_CMGD			10    //删除短信
#define AT_CPMS             11
#define AT_IPR              12
#define AT_CMGF1            13
//---------------------------------------


uchar  Send_AT_Command( uchar type,uchar *str_at );
uchar   READ_TEL(char* ptr1_at,uchar  id)  ;	