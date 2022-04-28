/****DS18B20������******���µ�������*******************
**********************************************************
****�ɶ�������1���������ԸĶ�ΪDQ
****
****���ڶ�ȡ��ǰ�����¶�
****a=ReadTemperature()���� aΪint�ͱ���
********************************************************/
#include<reg52.h>
#include <intrins.h>
#include <absacc.h>
//-----------�ɶ�����--------------------------
sbit DQ =P1^0;
//---------------------------------------------
void delay1820(unsigned int i)
{
     while(i--); 
}
//��ʼ������
Init_DS18B20(void)
{
// wtdg=!wtdg;
 DQ = 1;    //DQ��λ
 delay1820(8);  //������ʱ
 DQ = 0;    //��Ƭ����DQ����
 delay1820(80); //��ȷ��ʱ ���� 480us
 DQ = 1;    //��������
 delay1820(14);
// xbz=DQ;      //������ʱ�� ���x=0���ʼ���ɹ� x=1���ʼ��ʧ��
 delay1820(20);
 DQ=1;
}

//��һ���ֽ�
ReadOneChar(void)
{
unsigned char i=0;
unsigned char dat = 0;
//wtdg=!wtdg;
for (i=8;i>0;i--)
 {
  DQ = 0; // �������ź�
  dat>>=1;
  DQ = 1; // �������ź�
  if(DQ)
   dat|=0x80;
  delay1820(4);
 }
 return(dat);
}

//дһ���ֽ�
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
delay1820(4);
}

void Config18b20()
{
     Init_DS18B20();
     WriteOneChar(0xcc);  //skip rom
     WriteOneChar(0x4e);  //write scratchpad
     WriteOneChar(0x55);  //����
     WriteOneChar(0x00);  //����
     WriteOneChar(0x7f);     //set 11 bit (0.125)
     Init_DS18B20();
     WriteOneChar(0xcc);  //skip rom
     WriteOneChar(0x48);  //�����趨ֵ
     Init_DS18B20();
     WriteOneChar(0xcc);  //skip rom
     WriteOneChar(0xb8);  //�ص��趨ֵ
}

unsigned int ReadTemperature()
{
unsigned char a=0;
unsigned char b=0;
unsigned int t=0;
float tt=0;
unsigned int g=0;
EA=0;  //����������Ҫ���ж�
delay1820(4);
Config18b20();
Init_DS18B20();
WriteOneChar(0xCC); // ����������кŵĲ���
WriteOneChar(0x44); // �����¶�ת��
delay1820(80);
Init_DS18B20();
WriteOneChar(0xCC); //����������кŵĲ���
WriteOneChar(0xBE); //��ȡ�¶ȼĴ����ȣ����ɶ�9���Ĵ����� ǰ���������¶�
a=ReadOneChar();   //��ȡ�¶�ֵ��λ
b=ReadOneChar();   //��ȡ�¶�ֵ��λ
DQ=1;
EA=1;  //����������Ҫ���ж�
t=b;
t<<=8;
t=t|a;
tt=t*0.0625;        
g=tt*10+0.5;
return (g);
} 


