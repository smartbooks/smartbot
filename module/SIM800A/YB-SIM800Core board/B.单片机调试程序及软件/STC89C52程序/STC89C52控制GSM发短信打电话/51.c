#include <reg52.h>
#include <absacc.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char
#define Phone_connection  1    //���ڴ�绰״̬��־
#define Clear_Connect     0    //�绰�ѹҶ�״̬��־

sbit Phone_Call_Key=P1^0;     //��绰����
sbit Hang_up=P1^1;     //�ҵ绰����
sbit Send_SMS_Text_key=P1^2;     //��Text��ʽ���Ű���
sbit Send_SMS_PDU_key=P1^3;     //��PDU��ʽ���Ű���

char Calls_State_Flag=Clear_Connect;   //�绰״̬  
uchar code PhoneCall[]="15922260566";
uchar code PhoneCall2[]="02228214113";
uchar code SMS_send[]= "AT+CMGS=";  
uchar code ATH[]="ATH";
uchar code SMS_TXET[]= "AT+CMGF=1";//��������ΪTXET��ʽ 
uchar code SMS_PDU[]= "AT+CMGF=0";//��������ΪPDU��ʽ 
uchar code Sms[]="test ok!!";
uchar Byte_num[]="20";
uchar code Sms2_Pdu[]="0891683108200205F011000B815129220665F60008A70600204F60597D"; //�����ǣ�����

void delay(uint ms)// ��ʱ�ӳ���
{      
uchar i;                                     
while(ms--)
{
  for(i=0;i<120;i++);
}
}

/*********************************************************** 
�������ƣ�Print_Char 
�������ܣ����͵����ַ�     
          ��ڲ���:ch      ���ڲ������� 
***********************************************************/
void Print_Char(uchar ch)//���͵����ַ�
{
SBUF=ch; //���뻺����
while(TI==0); //�ȴ��������
TI=0; //�������
}

/*********************************************************** 
�������ƣ�Print_Str 
�������ܣ������ַ���     
          ��ڲ���:*str    ���ڲ������� 
***********************************************************/
Print_Str(uchar *str)//�����ַ���
{

while(*str!='\0') 
{
Print_Char(*str++);
    }

}
/*Print_Str(uchar *str,uint len)//�����ַ���,����Send_Char() len�ַ�������
{

while(len--)
{
Print_Char(*str++);
    }

}*/

 
/*********************************************************** 
�������ƣ�Ini_UART 
�������ܣ����ڳ�ʼ������ʱ����ʼ��     
          ��ڲ���:��     ���ڲ������� 
***********************************************************/ 
Ini_UART(void)//���ڳ�ʼ������ʱ����ʼ��
{
TMOD = 0x20;      //T1��ʽ2,����UART������ 
TH1 = 0xFD;       //UART����������:9600 
TL1 = 0xFD;
SCON = 0x50;  ////UART��ʽ1:8λUART;   REN=1:������� 
PCON = 0x00;

TF1 = 1;     // �жϱ�־λ
TR1 = 1;    // ������ʱ��1    
ES=1;       //�������п��ж�
ET0=1;      //�����жϹ���
EA=1;
}
 
/*********************************************************** 
�������ƣ�GSM_ATH 
�������ܣ��ҵ绰      
          ��ڲ���:��     ���ڲ������� 
***********************************************************/ 
void GSM_ATH()   
{   
     Print_Str(ATH); //���͹ҵ绰ָ��
     Print_Str("\r\n");   //���ͻس�
      
     delay(1000); 
     delay(1000); 
}  
	
/*********************************************************** 
�������ƣ�phone 
�������ܣ�����Ŀ��绰      
          ��ڲ���:��     ���ڲ������� 
***********************************************************/ 
void phone()    //��绰����
{  
	 Print_Str("ATD");
     Print_Str(PhoneCall2);   //���ʹ�绰ָ��+Ŀ��绰����
	 Print_Char(';');
     Print_Str("\r\n");   //���ͻس�
     delay(5000);
     delay(5000);
     
} 

/*********************************************************** 
�������ƣ�GSM_TEXT 
�������ܣ����÷�������ΪTXET��ʽ       
          ��ڲ���:��     ���ڲ������� 
***********************************************************/ 
 
void GSM_TEXT()   //����GSMΪTEXT��ʽ
{   
     Print_Str(SMS_TXET);   //��������ָ��
     Print_Str("\r\n");   //���ͻس�
      ES=1;
     delay(1000);
     delay(1000);
     delay(1000); 
}

/*********************************************************** 
�������ƣ�GSM_TEXT_SMS 
�������ܣ���������ΪTXET��ʽ�Ķ�������       
          ��ڲ���:��     ���ڲ������� 
***********************************************************/ 
 
void GSM_TEXT_SMS()  //����TEXT��ʽ����
{   

     Print_Str(SMS_send);
	 Print_Str(PhoneCall);
     Print_Str("\r\n");   //���ͻس�
     Print_Str(Sms);
     Print_Char(0x1a);  //���Ͷ�Ϣ���ݽ�������Ӧ��ASCIIΪ0x1a
     ES=1;
     delay(1000);
     delay(5000);   
} 
/*********************************************************** 
�������ƣ�GSM_PDU
�������ܣ����÷�������ΪPDU��ʽ       
          ��ڲ���:��     ���ڲ������� 
***********************************************************/ 
 
void GSM_PDU()    //����GSMΪPDU��ʽ
 {
     Print_Str(SMS_PDU);  //��������ָ��
     Print_Str("\r\n");   //���ͻس�
      ES=1;
     delay(1000); 
     delay(1000);
     delay(1000);
  }

/*********************************************************** 
�������ƣ�GSM_PDU_SMS
�������ܣ���������ΪPDU��ʽ�Ķ�������       
          ��ڲ���:��     ���ڲ������� 
***********************************************************/ 
void GSM_PDU_SMS()  //����PDU��ʽ����
{   

     Print_Str(SMS_send);
     Print_Str(Byte_num);
	// Print_Str(PhoneCall);
     Print_Str("\r\n");   //���ͻس�
     Print_Str(Sms2_Pdu);
     Print_Char(0x1a);  //���Ͷ�Ϣ���ݽ�������Ӧ��ASCIIΪ0x1a
     ES=1;
     delay(1000);
     delay(5000);   
}


/*********************************************************** 
�������ƣ�main
�������ܣ�������
***********************************************************/ 
void main()
{ 
Ini_UART();
while(1)
{  

   if(!Hang_up)  //�ҵ绰�����Ƿ���
        {
            GSM_ATH(); //���͹ҵ绰ָ��
            Calls_State_Flag=Clear_Connect;   
          }
   else if((!Phone_Call_Key)&&(Calls_State_Flag==Clear_Connect))
        {
          phone();  //����绰
         Calls_State_Flag=Phone_connection;
         }
    
     else if((!Send_SMS_Text_key)&&(Calls_State_Flag==Clear_Connect))
        {
          GSM_TEXT(); 	
		  GSM_TEXT_SMS();					 
         }
    
     else if((!Send_SMS_PDU_key)&&(Calls_State_Flag==Clear_Connect))
        {
         GSM_PDU();
         GSM_PDU_SMS();
         }
      
}
}