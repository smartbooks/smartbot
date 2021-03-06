/*-----------------------------------------------
  名称：音乐播放
  论坛：www.doflye.net
  编写：shifang
  日期：2009.5
  修改：无
  内容：
------------------------------------------------*/
#include<reg52.h>        //包含头文件，一般情况不需要改动?
                         //头文件包含特殊功能寄存器的定义
/*------------------------------------------------
                 硬件端口定义
------------------------------------------------*/
sbit      SPK=P0^2;  //定义音乐输出端口

unsigned char Timer0_H,Timer0_L,Time;
                         //世上只有妈妈好数据表
code unsigned char MUSIC[]={          6,2,3,      5,2,1,      3,2,2,    5,2,2,    1,3,2,    6,2,1,    5,2,1,
                                      6,2,4,      3,2,2,      5,2,1,    6,2,1, 	  5,2,2, 	3,2,2, 	  1,2,1,
                                      6,1,1,      5,2,1,      3,2,1, 	2,2,4, 	  2,2,3, 	3,2,1,    5,2,2,
                                      5,2,1,      6,2,1,      3,2,2, 	2,2,2,    1,2,4, 	5,2,3, 	  3,2,1,
                                      2,2,1,      1,2,1,      6,1,1, 	1,2,1, 	  5,1,6, 	0,0,0 
                                      };
                         // 音阶频率表 高八位
code unsigned char FREQH[]={
                                0xF2,0xF3,0xF5,0xF5,0xF6,0xF7,0xF8, 
                                0xF9,0xF9,0xFA,0xFA,0xFB,0xFB,0xFC,0xFC, //1,2,3,4,5,6,7,8,i
                                0xFC,0xFD,0xFD,0xFD,0xFD,0xFE,
                                0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFF,
                               } ;
                         // 音阶频率表 低八位
code unsigned char FREQL[]={
                                 0x42,0xC1,0x17,0xB6,0xD0,0xD1,0xB6,
                                 0x21,0xE1,0x8C,0xD8,0x68,0xE9,0x5B,0x8F, //1,2,3,4,5,6,7,8,i
                                 0xEE,0x44, 0x6B,0xB4,0xF4,0x2D, 
                                 0x47,0x77,0xA2,0xB6,0xDA,0xFA,0x16,
                                };
/*------------------------------------------------
 uS延时函数，含有输入参数 unsigned char t，无返回值
 unsigned char 是定义无符号字符变量，其值的范围是
 0~255 这里使用晶振12M，精确延时请使用汇编,大致延时
 长度如下 T=tx2+5 uS 
------------------------------------------------*/
void DelayUs2x(unsigned char t)
{   
 while(--t);
}
/*------------------------------------------------
 mS延时函数，含有输入参数 unsigned char t，无返回值
 unsigned char 是定义无符号字符变量，其值的范围是
 0~255 这里使用晶振12M，精确延时请使用汇编
------------------------------------------------*/
void DelayMs(unsigned char t)
{
     
 while(t--)
 {
     //大致延时1mS
     DelayUs2x(245);
	 DelayUs2x(245);
 }
}
/*------------------------------------------------
                节拍延时函数
 各调1/4节拍时间：
 调4/4  125ms
 调2/4  250ms
 调3/4  187ms
------------------------------------------------*/
void delay(unsigned char t)
{
    unsigned char i;
	for(i=0;i<t;i++)
	    DelayMs(250);
    TR0=0;
 }
/*------------------------------------------------
               定时器0中断
------------------------------------------------*/
void TIM0_ISR() interrupt 1
{
 TR0=0;      
 SPK=!SPK;
 TH0=Timer0_H;
 TL0=Timer0_L;
 TR0=1;
 }
/*------------------------------------------------
                歌曲处理函数
------------------------------------------------*/
void Song()
{
 TH0=Timer0_H;//赋值定时器时间，决定频率
 TL0=Timer0_L;
 TR0=1;       //打开定时器
 delay(Time); //延时所需要的节拍                      
 }
/*------------------------------------------------
                主函数
------------------------------------------------*/
 void main(void)
 {
 unsigned char k,i;
 TMOD|=0x01; //置定时器0工作方式1
 EA=1;       //打开全局中断
 ET0=1;      //打开定时0中断
 while(1)
      {
       i=0;  
       while(i<100)
	        {         //音乐数组长度 ，唱完从头再来        
            k=MUSIC[i]+7*MUSIC[i+1]-1;//去音符振荡频率所需数据
            Timer0_H=FREQH[k];
            Timer0_L=FREQL[k];
            Time=MUSIC[i+2];          //节拍时长
            i=i+3;
            Song();
           }
       } 
  }