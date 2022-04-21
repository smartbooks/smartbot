
//**** ���� ********************************************************************
/*******************************************************************************
 * �������Ի�����Դ�����ɹ��ݻ����Ϣ�Ƽ����޹�˾�ռ�
 * �����û��ο�ѧϰ������˾���ṩ�κμ���֧��
 * ����������Բο�������Ӧ����ʵ�ʹ����У���һ����ͨ������
 * ��˾��վ  http://www.wavesen.com/
 * �Ա���ַ  http://shop61591214.taobao.com/
*******************************************************************************/


// ICC-AVR application builder : 2007-5-20 17:21:25
// Target : M16
// Crystal: 7.3728Mhz
// AVR mega16 ���ڲ���
// AVR����������

#include <iom16v.h>
#include <macros.h>

#define F_CPU 7372800

const unsigned char buffer[]={"http://www.wavesen.com"};

//******************************************************************************
//���ڳ�ʼ��
void USART_Init( unsigned int baud )
{
  unsigned int tmp;
  
  tmp= F_CPU/baud/16-1;
  UBRRH = (unsigned char)(tmp>>8);
  UBRRL = (unsigned char)tmp;
  UCSRB = (1<<RXEN)|(1<<TXEN);
  UCSRC = (1<<URSEL)|(1<<USBS)|(1<<UCSZ0)|(1<<UCSZ1);
}

// ICC ���ɵĳ�ʼ��
// UART0 initialize
// desired baud rate: 9600
// actual: baud rate:9600 (0.0%)
// char size: 8 bit
// parity: Disabled
void uart0_init(void)
{
  UCSRB = 0x00; //disable while setting baud rate
  UCSRA = 0x00;
  UCSRC = BIT(URSEL) | 0x06;
  UBRRL = 0x2F; //set baud rate lo
  UBRRH = 0x00; //set baud rate hi
  UCSRB = 0x18;
}

//******************************************************************************
//������������ֱ�Ӵ������ֲ��Ͽ���������

// ���ݷ��͡�����5 ��8 λ����λ��֡��
void USART_Transmit( unsigned char data )
{
  while ( !( UCSRA & (1<<UDRE)) );
  UDR = data;
}

// ���ݽ��ա���5 ��8 ������λ�ķ�ʽ������ ��֡��
unsigned char USART_Receive( void )
{
  while ( !(UCSRA & (1<<RXC)) );
  return UDR;
}

//���������ַ�
void USART_Transmit_2( void )
{
  while ( !( UCSRA & (1<<UDRE)) );
  UDR = 'H';
  while ( !( UCSRA & (1<<UDRE)) );
  UDR = 'e';
  while ( !( UCSRA & (1<<UDRE)) );
  UDR = 'l';
  while ( !( UCSRA & (1<<UDRE)) );
  UDR = 'l';
  while ( !( UCSRA & (1<<UDRE)) );
  UDR = 'o';
}

//******************************************************************************
void main(void)
{
  unsigned char n=0,tmp=0;

  USART_Init(9600);  //������9600 ��ʼ������
  //uart0_init();
  
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
