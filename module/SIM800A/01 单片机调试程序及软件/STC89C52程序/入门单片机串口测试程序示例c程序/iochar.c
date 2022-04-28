#include <reg52.h>
#include <string.h>
sbit p2_0=P2^0;  //���ڵ�Ƭ���Ĵ����յ�ģ����Ϣʱ����LED��ʾ
sbit p1_4=P1^4;  //���ڵ�Ƭ���Ĵ����յ�ģ����Ϣʱ��ȷ���յ���Ϣ������
sbit P0_0=P0^0;
sbit P0_1=P0^1;
unsigned char ctrlz=0x1a;//����Ϣ������ctrl��z
unsigned char ATH[]="ATH\r";  //�Ҷϵ绰ָ��
unsigned char ATD[]="ATD02228214113;\r";  //��绰ָ��
unsigned char CMGF_1[]="AT+CMGF=1\r";   //���ö���ϢΪӢ�ķ�ʽ
unsigned char CSCA[]="AT+CSCA=\"+8613010761500\"\r";  //���ö������ĺ���
unsigned char CMGS[]="AT+CMGS=\"13462405067\"\r";       //����ĵ绰����

 
unsigned char str[20];
 unsigned char count=0;
 unsigned char flag=0;
 //unsigned char choice=0;

void Delay_xMs(unsigned long int x)
{
    unsigned int i,j;
    for( i =0;i < x;i++ )
    {
        for( j =0;j<3;j++ );
    }
}
void send_modem_string(unsigned char *modem_string)//ָ���ַ�������ָ��
{
  while(*modem_string)
      {
	    SBUF = *modem_string;
	    while(TI==0);
		TI=0;
	    modem_string++;
	  }
}

void send_SMS_english()//����Ӣ�Ķ���Ϣ������P0��1Ϊ�͵�ƽʱ��Ч����ִ�иú���
{
   send_modem_string(CMGF_1); //����AT+CMGF=1ָ��,����ΪӢ��
   Delay_xMs(1000);
   send_modem_string(CSCA);//����AT+CSCA="+8613010761500"ָ����ö���Ϣ���ĺ���
   Delay_xMs(1000);
   send_modem_string(CMGS); //����AT+CMGS=15939187275ָ����ö���Ϣ���͵����ֻ�����
   Delay_xMs(1000);
   send_modem_string("The highest temperature is");//���Ͷ���Ϣ���ݣ���û��ctrl��z������
   send_modem_string("13 ^C");
//   SBUF=0x5F;//
//   while(TI==0);
 //  TI=0;
 //  SBUF=0x53;//
 //  while(TI==0);
 //  TI=0;

   SBUF=ctrlz;//���Ͷ���Ϣ������ctrl��z,���Ǳ���ġ�
   while(TI==0);
   TI=0;
}


void serial_recieve(unsigned char a)
{
    while(RI==1)
		{
	           RI=0;
		   str[count]=SBUF;
		   count++;
  		   while((RI==0)&(count<a));//��Ƭ���������ݵĻ���ʱ�䣬���û��ʱ�仺�壬���޷���������ģ�鷢�͸���Ƭ���ķ�����Ϣ��
		}
	count=0;

    //Delay_xMs(1000);


//	return 1;
}

void main()
{
  ///////����:���ڳ�ʼ��,������115200(9600)����ʽ1///////
  TMOD = 0x20;
  PCON = 0x00;
  SCON = 0x50;
  TH1 = 0xf3;
  TL1 = 0xf3;
  TR1 = 1;
  // EA=1;     //��CPU�жϺʹ��п��ж�
 // ES=1;


  Delay_xMs(1000);

  send_modem_string(ATD);   //��绰


  while(1)
   {
      while(RI==1)  //һ���յ�ģ��ķ�����Ϣ������P2.0�˿ڵ�LED 1000ms��Ϩ��
	     {
		  p2_0=0;
		  Delay_xMs(1000);
		  p2_0=1;
                  RI = 0;
		 }
	  //P0_0=0;
      if(P0_0==0)
        {
             send_modem_string(ATH);//�Ͽ��绰ͨ������
	     serial_recieve(8);//��������Ӧ�ã����ڵ��ڣ������ַ�����3����5������С�ڵ��ڣ��������ַ�����3����7����
         if((str[6]=='O')&(str[7]=='K'))
		   {              //�˴���ӶϿ��绰�ɹ�ʱ����Ļ����ʾ'�Ͽ��ɹ�'����Ϣ
		    p1_4=0;
		    Delay_xMs(1000);
		    p1_4=1;
	       }
		 else
		   {
		        //�˴��Ͽ��绰���ɹ�ʱ����Ϣ
		   }
		 RI=0;
         P0_0=1;
        }
     if(P0_1==0)
     {
       send_SMS_english();

       P0_1=1;
     }

   }
}

