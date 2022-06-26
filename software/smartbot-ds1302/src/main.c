
#include "delay.h"
#include "uart.h"
#include "DS1302.h"
#include <stdio.h>


void displayTime();

void main(){
	
	InitUART();

	log("Welcome to DS1302 Demo");

	DelayMs(250);

	log("ds1302 init");

	Ds1302_Init();

	displayTime();

    log("ds1302 write time...\r\n世纪 年-月-日 时:分:秒 星期");
	
	Ds1302_Write_Time();

	while(1){

		Ds1302_Read_Time();
		
		Delay1000ms();

		displayTime();
	};

}

void displayTime(){

	//空 年-月-日 时:分:秒 星期
	printf("%bu %bu-%Bu-%Bu %Bu:%bu:%bu %bu\r\n", 
				time_buf1[0],
				time_buf1[1], 
				time_buf1[2], 
				time_buf1[3], 
				time_buf1[4], 
				time_buf1[5], 
				time_buf1[6], 
				time_buf1[7]);

}




