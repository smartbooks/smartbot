#pragma OPTIMIZE(6,size)   	//优化级别与优化方式
//#pragma LARGE 				//编译模式 SMALL  COMPACT LARGE


#include <ctype.h>			//字符函数
#include <stdio.h>		//一般IO/函数
#include <string.h>			//字符串函数
//#include <stdlib.h>			//标准函数
//#include <mach.h>			//数学函数
#include <intrins.h>		//内部函数
 #include <stdarg.h>		//变量参数表


typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;

     
//-----------------------------------------------------------------------------------------
 

 	#define _Nop()    _nop_() 
 	#define IO_OUT			P3_6 
	sbit IO_IGT 	=P3^4;
	//外部中断
	sbit IO_P35		=P3^2;
	sbit IO_P24		=P2^4;
	sbit IO_P25		=P2^5;
    sbit IO_P27		=P2^7;

    sbit LED_S0 = P0^0;
    sbit LED_S1 = P0^1;


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
#define SMS_CMGS			14
#define CMGS_MUB			15
//---------------------------------------


uchar  Send_AT_Command( uchar type,uchar *str_at ,uchar *str_code);
uchar   READ_TEL(char* ptr1_at,uchar  id)  ;	