
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

    log("ds1302 write time...");
	
	Ds1302_Write_Time();

	while(1){

		Delay1000ms();

		Ds1302_Read_Time();
		
		Delay1000ms();
		Delay1000ms();

		displayTime();
	};

}

void displayTime(){

	printf("%bd %bd %bd %bd %bd %bd %bd\r\n", time_buf1[1], time_buf1[2], time_buf1[3], time_buf1[4], time_buf1[5], time_buf1[6], time_buf1[7]);

	//printf("%bd %bd %bd %bd\r\n", time_buf1[4], time_buf1[5], time_buf1[6], time_buf1[7]);

}




