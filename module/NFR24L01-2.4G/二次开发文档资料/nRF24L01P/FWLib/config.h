
#ifndef		__CONFIG_H
#define		__CONFIG_H


/*********************************************************/

#define MAIN_Fosc		22118400L	//定义主时钟
//#define MAIN_Fosc		12000000L	//定义主时钟
//#define MAIN_Fosc		11059200L	//定义主时钟
//#define MAIN_Fosc		 5529600L	//定义主时钟
//#define MAIN_Fosc		24000000L	//定义主时钟


/*********************************************************/

#include   "STC15Fxxxx.H"    //STC15系列内置支持SPI
//#include <STC90C5xAD.H>    //STC90系列芯片不支持SPI
#include "typedef.h"

/**************************************************************************/

#define Main_Fosc_KHZ	(MAIN_Fosc / 1000)

/***********************************************************/

#endif
