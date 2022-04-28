/****DS1302������******���µ�������*******************
**********************************************************
****�ɶ�������3���������ԸĶ�
****
****���ڴ洢�Ͷ�ȡ��ǰʱ��
****
********************************************************/
#include<reg52.h>
#include <intrins.h>
#include <absacc.h>
#define uint unsigned int
#define uchar unsigned char
#define DS1302_SECOND 0x80
#define DS1302_MINUTE 0x82
#define DS1302_HOUR 0x84 
#define DS1302_WEEK 0x8A
#define DS1302_DAY 0x86
#define DS1302_MONTH 0x88
#define DS1302_YEAR 0x8C
//-----------�ɶ�����--------------------------
sbit DS_REST=P1^6;
sbit DS_SCLK=P1^4;
sbit DS_SDATA=P1^5;
//---------------------------------------------
/*
1����Ĵ��������λҪ����0
2������ʱ����������û��������������
3����Ƭ��ϵͳ�Ƿ����������񣬸�λ���ܲ�����������������
4��DS1302ʱ������������ü�10k���ҵ���������
5����д������ʱ��ע���Ǹ�REST��Ҫ�����óɵ�Ȼ�������óɸߵ�ƽ
���뵽��Ҳ����ô���ˣ��ؼ�����Ҫ��ס�����ֲ᲻�ţ�����ϸ����������������ٸ�������д�Ͷ��ĺ������ο������������������ġ�ף����� 
*/
void delay_1302(unsigned int cnt)//��ʱ
{
 while(--cnt);
}
void Write_DS_Byte(unsigned char DS_Byte)
{
    unsigned char i=0;
 for(i=0;i<8;i++)
 {
     if((DS_Byte&0x01)==0x01)
         DS_SDATA=1;
  else
         DS_SDATA=0; 
    delay_1302(10);
        DS_SCLK=1;
       delay_1302(10);
        DS_SCLK=0;
        DS_Byte=(DS_Byte>>1);
 }
}
void Wirte_DS1302(unsigned char DS_Addr,unsigned char DS_Data)
{
    DS_REST=0;
    DS_SCLK=0;
    DS_REST=1;
    Write_DS_Byte(DS_Addr);
    Write_DS_Byte(DS_Data);
    DS_SCLK=1;
    DS_REST=0;    
}
unsigned char Read_DS_Byte()    //ֻ����8λ���ݵ�λ�ƶ�ȡ
{
    unsigned char i=0;
    unsigned char ClockValue=0;

    for(i=0;i<7;i++)
    {
        if(DS_SDATA==1)
        {
            ClockValue=(ClockValue|0x80);
        }
        else
        {
            ClockValue=(ClockValue&0x7f);
        }
        ClockValue=(ClockValue>>1);
        DS_SCLK=1;
        delay_1302(2);
        DS_SCLK=0;
    }
    return (ClockValue);
}
unsigned char ReadTimeValue(unsigned char DS_Addr) //��ָ����ַ�ļĴ�������
{
    unsigned char Value;
    DS_REST=0;
    DS_SCLK=0;
    DS_REST=1;
    Write_DS_Byte(DS_Addr);
    Value=Read_DS_Byte();
    DS_SCLK=1; 
    DS_REST=0;
    return(Value); 
} 

void InitDS1302(void)
{
Wirte_DS1302(0x8E,0x00);
Wirte_DS1302(DS1302_YEAR,0x09);
Wirte_DS1302(DS1302_MONTH,0x04);
Wirte_DS1302(DS1302_DAY,0x06);
Wirte_DS1302(DS1302_WEEK,0x01);
Wirte_DS1302(DS1302_HOUR,0x01);
Wirte_DS1302(DS1302_MINUTE,0x02);
Wirte_DS1302(DS1302_SECOND,0x20);
Wirte_DS1302(0x8E,0x80);
}

