  
#include <ctype.h>			//字符函数
#include <stdio.h>		//一般IO/函数
#include <string.h>			//字符串函数 
#include <intrins.h>		//内部函数 

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;
  

//=============================================================================
//                  基本的常用常量
//=============================================================================
#define ON		1
#define OFF		0
#define TRUE	1 
#define FALSE	0



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

 
