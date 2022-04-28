#include<reg52.h>
#include <intrins.h>
#include <absacc.h>

//-----------------------------------------------------------
//
//18B20驱动文件
//DQ设置为P0.0口
//
//------------------------------------------------------------
sbit DQ =P1^0;
void delay1820(unsigned int i)
{
     while(i--); 
}
//初始化函数
Init_DS18B20(void)
{
// wtdg=!wtdg;
 DQ = 1;    //DQ复位
 delay1820(8);  //稍做延时
 DQ = 0;    //单片机将DQ拉低
 delay1820(80); //精确延时 大于 480us
 DQ = 1;    //拉高总线
 delay1820(14);
// xbz=DQ;      //稍做延时后 如果x=0则初始化成功 x=1则初始化失败
 delay1820(20);
 DQ=1;
}

//读一个字节
ReadOneChar(void)
{
unsigned char i=0;
unsigned char dat = 0;
//wtdg=!wtdg;
for (i=8;i>0;i--)
 {
  DQ = 0; // 给脉冲信号
  dat>>=1;
  DQ = 1; // 给脉冲信号
  if(DQ)
   dat|=0x80;
  delay1820(4);
 }
 return(dat);
}

//写一个字节
WriteOneChar(unsigned char dat)
{
 unsigned char i=0;
// wtdg=!wtdg;
 for (i=8; i>0; i--)
 {
  DQ = 0;
  DQ = dat&0x01;
  delay1820(5);
  DQ = 1;
  dat>>=1;
 }
delay_1302(4);
}

void Config18b20()
{
     Init_DS18B20();
     WriteOneChar(0xcc);  //skip rom
     WriteOneChar(0x4e);  //write scratchpad
     WriteOneChar(0x55);  //上限
     WriteOneChar(0x00);  //下限
     WriteOneChar(0x7f);     //set 11 bit (0.125)
     Init_DS18B20();
     WriteOneChar(0xcc);  //skip rom
     WriteOneChar(0x48);  //保存设定值
     Init_DS18B20();
     WriteOneChar(0xcc);  //skip rom
     WriteOneChar(0xb8);  //回调设定值
}

unsigned int ReadTemperature()
{
unsigned char a=0;
unsigned char b=0;
unsigned int t=0;
float tt=0;
unsigned int g=0;
Config18b20();
Init_DS18B20();
WriteOneChar(0xCC); // 跳过读序号列号的操作
WriteOneChar(0x44); // 启动温度转换
delay_1302(80);
Init_DS18B20();
WriteOneChar(0xCC); //跳过读序号列号的操作
WriteOneChar(0xBE); //读取温度寄存器等（共可读9个寄存器） 前两个就是温度
a=ReadOneChar();   //读取温度值低位
b=ReadOneChar();   //读取温度值高位
DQ=1;

t=b;
t<<=8;
t=t|a;
tt=t*0.0625;        
g=tt*10+0.5;
return (g);
} 


