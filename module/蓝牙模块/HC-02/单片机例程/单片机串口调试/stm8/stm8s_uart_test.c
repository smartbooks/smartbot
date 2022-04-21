
//**** ���� ********************************************************************
/*******************************************************************************
 * �������Ի�����Դ�����ɹ��ݻ����Ϣ�Ƽ����޹�˾�ռ�
 * �����û��ο�ѧϰ������˾���ṩ�κμ���֧��
 * ����������Բο�������Ӧ����ʵ�ʹ����У���һ����ͨ������
 * ��˾��վ  http://www.wavesen.com/
 * �Ա���ַ  http://shop61591214.taobao.com/
*******************************************************************************/


//�ṩMCU�Ĵ����Ķ���
#include "STM8S103F.h"

const unsigned char buffer[]={"http://www.wavesen.com"};

//******************************************************************************
//���ڳ�ʼ��
void USART_Init( unsigned int baud )
{
  CLK_CKDIVR=0;  //16MHz
  
  //������16MHzʱ��ʱΪ9600
  UART1_BRR2=0x02;
  UART1_BRR1=0x68;
  //ֹͣλ
  UART1_CR3 &= 0xcf;  //1 stop bit
  //��żУ��  ����λ��
  UART1_CR1 &= 0xe9;  //none
  //ʹ���շ�
  UART1_CR2|=0x0c;
}

//******************************************************************************
// ���ݷ���
void USART_Transmit( unsigned char data )
{
  while(!(UART1_SR&0x80));
  UART1_DR = data;
}
// ���ݽ���
unsigned char USART_Receive( void )
{
  while ( !(UART1_SR&0x20) );
  return UART1_DR;
}
//���������ַ�
void USART_Transmit_2( void )
{
  while(!(UART1_SR&0x80));
  UART1_DR = 'H';
  while(!(UART1_SR&0x80));
  UART1_DR = 'e';
  while(!(UART1_SR&0x80));
  UART1_DR = 'l';
  while(!(UART1_SR&0x80));
  UART1_DR = 'l';
  while(!(UART1_SR&0x80));
  UART1_DR = 'o';
}

//******************************************************************************
void main(void)
{
  unsigned char n=0,tmp=0;

  USART_Init();  //������9600 ��ʼ������
  
  //���ݷ���
  for(i=0;n<20;n++){  //��������������ַ���
    USART_Transmit(buffer[n]);
  }
  USART_Transmit(0x0d);  //����һ���س�
  USART_Transmit(0x0a);  //����һ������
  
  //���ݽ���
  while(1){
    
    if(UCSRA&(1<<RXC)){  //������ջ�����������
      
      tmp=USART_Receive();  //��������
      USART_Transmit(tmp);  //��������
      
      if(tmp=='a'){  //�ظ����յ������ݣ������a���ٷ�һ��OK����
        USART_Transmit('O');
        USART_Transmit('K');
      }
      
      if(tmp=='A'){  //�ظ����յ������ݣ������A���ٷ�һ��Hello����
        USART_Transmit_2();
      }
    }
  }
}
