#include <reg52.h>
#define DATAPORT P0                             //����P0��ΪLCDͨѶ�˿�
sbit RS=P3^2;								 //����/�����
sbit RW=P3^3;								 //��/дѡ���
sbit E=P3^4;
sbit RST=P3^7;
sbit PSB=P3^5;

void delay_12864(uint x)
{
  uint i,j;
  for(i=0;i<x;i++)
   for(j=0;j<110;j++);


}
////////////////////////////////////////
void LCM12864_TestBusy()
{
RW=1;
RS=0;
E=1;
while(DATAPORT&0X08);
}


/********************д����**********************/
//д�����ӳ���
//
/************************************************/
void send_com(uchar cmd)/*д����*/
{ LCM12864_TestBusy();
   delay_12864(10);
   RS = 0;
   RW = 0;
   DATAPORT = cmd;
   E = 1;
   E = 0;
}
/********************д����**********************/
//д�����ӳ���
//
/************************************************/
void send_data(uchar dat)
{ 
   delay_12864(1);
   RS = 1;
   RW = 0;
   DATAPORT = dat;
   E = 1;
   E = 0;
}
/********************��ʼ��**********************/
//��λ��ͨѶ��ʽѡ��
/************************************************/
void lcd_init(void)
{ 
   
   RST=1;//��λRST=1
      PSB = 1;       //ͨѶ��ʽΪ����PSB = 1
        //send_com(0x34);//34H--����ָ�����
   send_com(0x30);//�������ã�һ����8λ���ݣ�����ָ�
   send_com(0x0C);//0000,1100 ������ʾ���α�off���α�λ��off
   send_com(0x01);//0000,0001 ��DDRAM
   send_com(0x02);//0000,0010 DDRAM��ַ��λ
   send_com(0x80);//1000,0000 �趨DDRAM 7λ��ַ000��0000����ַ������AC
}

/*******************************************************************/
//          ������ʾλ��    xpos(0~15),tpos(0~3)
/*******************************************************************/
void LCD_Set_xy(uchar xpos,uchar ypos)
 {
    uchar address;
    switch(ypos)
    {
     case 0:
          address = 0x0080 + xpos;   //��һ�е�ַΪ��0x0080----0x0087
          break;                  
     case 1:
          address = 0x0090 + xpos;   //�ڶ��е�ַΪ��0x0090----0x0097
          break;
     case 2:
       address = 0x0088 + xpos;   //�����е�ַΪ��0x0088----0x008F
          break;
     case 3:
       address = 0x0098 + xpos;   //�����е�ַΪ��0x0098----0x009F
          break;
     default:
       address = 0x0080 + xpos;   //Ĭ�ϵ�һ��
      }
       send_com(address);
 } 
void Print_String(char *str)  
 {  
      while( (*str)!='\0' )
      {  
         send_data(*str);
         str++;
      }
 }

 void LCD_12864_DISPLAY(uchar X,uchar Y,uchar *str)
{ EA=0;
 delay_12864(1);
 LCD_Set_xy(X,Y);
    Print_String(str);
EA=1;
}
 

 void LCD_12864_DISPLAY_ZU(uchar x,uchar y,uchar str[])
{ 
   uchar n; 
      LCD_Set_xy(x,y);                                  
   for(n=0;n<sizeof(str)-1;n++)
       { 
        send_data(str[n]);
       }
 }

/********************����************************/
//����
/************************************************/
void clr_lcd(void)
{
     send_com(0x01);

}









