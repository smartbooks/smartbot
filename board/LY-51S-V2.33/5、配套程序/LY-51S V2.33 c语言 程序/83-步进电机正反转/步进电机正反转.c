/*-----------------------------------------------
  ���ƣ��������
  ��̳��www.doflye.net
  ��д��shifang
  ���ڣ�2009.5
  �޸ģ���
  ���ݣ����������ڲ���4�ಽ������������� ������ת ʹ��1������
        

 ------------------------------------------------*/

#include <reg52.h>



sbit A1=P1^0; //���岽��������Ӷ˿�
sbit B1=P1^1;
sbit C1=P1^2;
sbit D1=P1^3;


#define Coil_A1 {A1=1;B1=0;C1=0;D1=0;}//A��ͨ�磬������ϵ�
#define Coil_B1 {A1=0;B1=1;C1=0;D1=0;}//B��ͨ�磬������ϵ�
#define Coil_C1 {A1=0;B1=0;C1=1;D1=0;}//C��ͨ�磬������ϵ�
#define Coil_D1 {A1=0;B1=0;C1=0;D1=1;}//D��ͨ�磬������ϵ�
#define Coil_AB1 {A1=1;B1=1;C1=0;D1=0;}//AB��ͨ�磬������ϵ�
#define Coil_BC1 {A1=0;B1=1;C1=1;D1=0;}//BC��ͨ�磬������ϵ�
#define Coil_CD1 {A1=0;B1=0;C1=1;D1=1;}//CD��ͨ�磬������ϵ�
#define Coil_DA1 {A1=1;B1=0;C1=0;D1=1;}//D��ͨ�磬������ϵ�
#define Coil_OFF {A1=0;B1=0;C1=0;D1=0;}//ȫ���ϵ�

unsigned char Speed;
/*------------------------------------------------
 uS��ʱ����������������� unsigned char t���޷���ֵ
 unsigned char �Ƕ����޷����ַ���������ֵ�ķ�Χ��
 0~255 ����ʹ�þ���12M����ȷ��ʱ��ʹ�û��,������ʱ
 �������� T=tx2+5 uS 
------------------------------------------------*/
void DelayUs2x(unsigned char t)
{   
 while(--t);
}
/*------------------------------------------------
 mS��ʱ����������������� unsigned char t���޷���ֵ
 unsigned char �Ƕ����޷����ַ���������ֵ�ķ�Χ��
 0~255 ����ʹ�þ���12M����ȷ��ʱ��ʹ�û��
------------------------------------------------*/
void DelayMs(unsigned char t)
{
     
 while(t--)
 {
     //������ʱ1mS
     DelayUs2x(245);
	 DelayUs2x(245);
 }
}



void Delay1ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 15;
	j = 90;
	do
	{
		while (--j);
	} while (--i);
}
void Delay200us()		//@11.0592MHz
{
	unsigned char i, j;

	i = 3;
	j = 221;
	do
	{
		while (--j);
	} while (--i);
}

void DelayUs(unsigned char t)
{
 while(t--)
 {
     Delay200us();
 }
}
void DelayMs2(unsigned char t)
{
 while(t--)
 {
     Delay1ms();
 }
}

/*------------------------------------------------
                    ������
------------------------------------------------*/
main()
{

while(1){

 unsigned int i=512;//��תһ��ʱ��
 Speed=10;
 Coil_OFF
 while(i--)  //����
  {         
     Coil_A1                //����Coil_A1  ��{A1=1;B1=0;C1=0;D1=0;}����
     DelayUs(Speed);         //�ı�����������Ե������ת�� ,
                             //����ԽС��ת��Խ��,����ԽС
     Coil_B1
     DelayUs(Speed);
     Coil_C1
     DelayUs(Speed);
     Coil_D1
     DelayUs(Speed);
  }
  Coil_OFF
  i=512;
  while(i--)//����
  {
     Coil_D1                //����Coil_A1  ��{A1=1;B1=0;C1=0;D1=0;}����
     DelayUs(Speed);         //�ı�����������Ե������ת�� ,
	                         //����ԽС��ת��Խ��,����ԽС

     Coil_C1
     DelayUs(Speed);

     Coil_B1
     DelayUs(Speed);

     Coil_A1
     DelayUs(Speed);
  }

}
}
