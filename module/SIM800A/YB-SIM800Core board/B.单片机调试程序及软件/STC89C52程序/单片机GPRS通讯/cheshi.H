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
#define  CPU_TYPE2   2    //定义CPU的类型
#define  W78E58      1 
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
 	#define BELL  P1_1
 	#define IO_OUT			P1_0
	#define GPS_ON			P0_0 //GPS开关
	sbit IO_IGT 	=P0^6;
	//外部中断
	sbit IO_P35		=P3^2;
	sbit IO_P24		=P2^4;
	sbit IO_P25		=P3^7;
    sbit IO_P27		=P2^7;

    sbit LED_S0 = P1^4;
    sbit LED_S1 = P1^5;


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
#define VOICE_DIAL			0  	//语音拨号
#define AT_CSCA				1   //读短信中心号码
#define DTMF_DIAL       	2	//通话中拨号
#define RESET_TC35			3	//复位TC35
#define TC35_INIT 			4	//TC35初始化命令
#define CALL_ID 			5	//读来电显示
#define SWITCHOFF_TC35 		7	//关机

#define AT_CPIN  			9	//检查当前是否要输入PIN码
#define AT_CGMM    			10	//
#define REGISTER_CHECK 		11	//检查是否登陆成功
#define SIGNAL_CHECK 		12	//读取信号质量
#define HOOKOFF 			13	//挂机,停止通话
#define PICK_UP         	14 	//摘机
#define COPS_UP				15
#define VOICE_HANDSET       16  //设为手柄方式
#define DATATIME_SET  		17	//设置TC35的内部时钟
#define DATATIME_READ 		18	//读出TC35的内部时钟
#define SMS_CMGL			19  //读所有短信
#define AT_CMGF0            20   //中文短信方式

#define SMS_CNMI 			21	//SMS 接收方式
#define SMS_CMGS			22  //SMS 短信发送
#define SIM_CARD_CHECK      23  //查寻是否插入SIM卡
#define AT_COMMAND          24
#define SMS_CMGR			25   //读一个短信
#define PHONE_DL_CHECK      26  //查寻已拨电话
#define PHONE_READ          27  //读电话本
#define PHONE_WRITE         28  //写电话本
#define SMS_CMGD			29    //删除短信
#define MONITOR_ID			30    //读小区
#define PHONE_MC_CHECK      31  //已接来电
#define PHONE_RC_CHECK      32  //未接来电

//-----GPRS 专用--------------------------------
#define AT_CIPSHUT          40
#define AT_CLPORT           41
#define AT_CSTT             42
#define AT_CIICR            43
#define AT_CIFSR            44
#define AT_CIPCLOSE         45
#define AT_CDNSGIP          46
#define AT_CDNSORIP         47
#define AT_CIPDPDP          48
#define AT_CIPHEAD          49
#define AT_CIPATS           50
#define AT_CIPSERVER        51
#define AT_CIPCSGP          52
#define TC35_IMEI           53
#define CMGS_MUB			54
#define	CMGS_SM				55
#define	SMSS_ID				56
#define PHONE_SET       	57
#define TC35_SBC	       	58  //检查电池电量
#define AT_CREG             59
#define AT_CDNSCFG          60
#define AT_CIPCCON          61
#define AT_CPBS             62
#define AT_CPBF             63
#define AT_CPMS             64
#define AT_CIPSEND          65
#define AT_CIPSTART         66
#define AT_CIPSTATUS        67
#define AT_CFUN             68
#define AT_CLTS             69
#define TC35_IPR            70
#define AT_CALARM           71
#define AT_IPR              72
#define SMS_CMGF            73
#define AT_COLP             74
#define AT_CMGF1            75
//---------------------------------------


//===================================================
// 发送短消息内容方式
//====================================================
#define   	smss_text_1    	1  //smss_tel smss_alarm   smss_end smss_start   smss_para
#define 	smss_text_tmp	2
#define		smss_pud_1		3
#define		smss_start		4
#define		smss_pud_2		5
// system_server
#define	SYS_PDU_1					1	//
#define	SYS_RING					2	//
#define SYS_TEXT_1					3	//
#define SYS_KEY_1	 				4	//
#define	SYS_SMSR					5	//
#define SYS_KEY_2					6	//
#define SYS_SMSS					7	//
#define SYS_TEL						8    //
#define SYS_DIAL                    9


uchar  send_sms(char* ptr1_at,char* ptr1_code,char* ptr_tel,uchar id);

uchar  Send_AT_Command( uchar type,uchar *str_at ,uchar *str_code);
void  Sys_Init(void);
void Initialize_Model(char* ptr1_at,char* ptr1_code) ;
uchar strsearch(uchar *ptr2,uchar *ptr1_at);
uchar PHONE_RD(char* ptr1_at,uchar *str,uchar id ) ;
uchar   READ_TEL(char* ptr1_at,uchar  id); 
void GPS_READ(char* ptr1_at,char* ptr1_code,char* ptr_tel,uchar id);
