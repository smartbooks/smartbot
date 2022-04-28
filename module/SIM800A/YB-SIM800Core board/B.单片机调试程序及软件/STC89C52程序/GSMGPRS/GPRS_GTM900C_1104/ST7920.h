//���ô��ںͲ��ڵķ�ʽ��ST7920����

#ifndef __ST7920_H__
#define __ST7920_H__

#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include <string.h> 
//#include <macros.h> 


//----------------------------------------------------------------
//ÿ����ʾ8�����֣�16���ַ������ֵ�ַ����Ϊ0x80,00x81,0x82,......
//�ַ�Ҳʹ����Щ��ַ��Ҳ�����ַ���Ӧ�ں������ʹ����Щ��ַ��
//��Ӧ�ں����Ҳ����û�е�ַ������ʱֻ��ʹ��д�ַ�����������ǰһ���ַ���
//�ڽ���дһ��
//----------------------------------------------------------------

#define PARALLEL	1

#if PARALLEL == 1

	#define LCD_CTRL_PORT 	PORTA
	#define LCD_CTRL_DDR 	DDRA
	#define LCD_CTRL_PIN 	PINA											
	#define LCD_RS	5  //CS   ��������
	#define LCD_RW	6	//SIO  ��������
	#define LCD_E	7  //SCK  ��������
	#define LCD_RST	0
	//#define LCD_PSB	5

	#define LCD_DATA_DDR	DDRC
	#define LCD_DATA_PIN	PINC
	#define LCD_DATA_PORT	PORTC


	#ifdef LCD_RST
		#ifdef LCD_PSB
		#define LCD_CTRL_OUT()	LCD_CTRL_DDR |= (1<<(LCD_RS))|(1<<(LCD_RW))|(1<<(LCD_E))|(1<<(LCD_RST))|(1<<(LCD_PSB))
		#else
		#define LCD_CTRL_OUT()	LCD_CTRL_DDR |= (1<<(LCD_RS))|(1<<(LCD_RW))|(1<<(LCD_E))|(1<<(LCD_RST))
		#endif	//#ifdef LCD_PSB
	#else
		#ifdef LCD_PSB
		#define LCD_CTRL_OUT()	LCD_CTRL_DDR |= (1<<(LCD_RS))|(1<<(LCD_RW))|(1<<(LCD_E))|(1<<(LCD_PSB))
		#else
		#define LCD_CTRL_OUT()	LCD_CTRL_DDR |= (1<<(LCD_RS))|(1<<(LCD_RW))|(1<<(LCD_E))
		#endif	//#ifdef LCD_PSB
	#endif	//#ifdef LCD_RST


	#define LCD_RS_LOW()	LCD_CTRL_PORT &= ~(1<<LCD_RS)
	#define LCD_RS_HIGH()	LCD_CTRL_PORT |= (1<<LCD_RS)

	#define LCD_RW_LOW()	LCD_CTRL_PORT &= ~(1<<LCD_RW)
	#define LCD_RW_HIGH()	LCD_CTRL_PORT |= (1<<LCD_RW)

	#define LCD_E_LOW()		LCD_CTRL_PORT &= ~(1<<LCD_E)
	#define LCD_E_HIGH()	LCD_CTRL_PORT |= (1<<LCD_E)

	#ifdef LCD_RST
	#define LCD_RST_LOW()	LCD_CTRL_PORT &= ~(1<<LCD_RST)
	#define LCD_RST_HIGH()	LCD_CTRL_PORT |= (1<<LCD_RST)
	#endif	//#ifdef LCD_RST

	#ifdef LCD_PSB
	#define LCD_PSB_LOW()	LCD_CTRL_PORT &= ~(1<<LCD_PSB)
	#define LCD_PSB_HIGH()	LCD_CTRL_PORT |= (1<<LCD_PSB)
	#endif	//#ifdef LCD_PSB



	#define LCD_DATA_IN()	LCD_DATA_DDR &= ~0xFF
	#define LCD_DATA_OUT()	LCD_DATA_DDR |= 0xFF
	#define LCD_DATA_GET()	LCD_DATA_PIN

#else	//PARALLEL == 0,���ڲ���


	#define LCD_CTRL_PORT 	PORTA
	#define LCD_CTRL_DDR 	DDRA
	#define LCD_CTRL_PIN 	PINA											
	#define LCD_CS	5	//RS   ��������
	#define LCD_SIO	6	//RW  ��������
	#define LCD_SCK	7	//E  ��������
	#define LCD_RST	0
	//#define LCD_PSB	5

	#define LCD_DATA_DDR	DDRC
	#define LCD_DATA_PIN	PINC
	#define LCD_DATA_PORT	PORTC


	#ifdef LCD_RST
		#ifdef LCD_PSB
		#define LCD_CTRL_OUT()	LCD_CTRL_DDR |= (1<<(LCD_CS))|(1<<(LCD_SCK))|(1<<(LCD_RST))|(1<<(LCD_PSB))
		#else
		#define LCD_CTRL_OUT()	LCD_CTRL_DDR |= (1<<(LCD_CS))|(1<<(LCD_SCK))|(1<<(LCD_RST))
		#endif	//#ifdef LCD_PSB
	#else
		#ifdef LCD_PSB
		#define LCD_CTRL_OUT()	LCD_CTRL_DDR |= (1<<(LCD_CS))|(1<<(LCD_SCK))|(1<<(LCD_PSB))
		#else
		#define LCD_CTRL_OUT()	LCD_CTRL_DDR |= (1<<(LCD_CS))|(1<<(LCD_SCK))
		#endif	//#ifdef LCD_PSB
	#endif	//#ifdef LCD_RST

	#define LCD_SIO_IN()	LCD_CTRL_DDR	&= ~(1<<LCD_SIO)
	#define LCD_SIO_OUT()	LCD_CTRL_DDR	|= (1<<LCD_SIO)


	#define LCD_CS_LOW()	LCD_CTRL_PORT &= ~(1<<LCD_CS)
	#define LCD_CS_HIGH()	LCD_CTRL_PORT |= (1<<LCD_CS)

	#define LCD_SCK_LOW()	LCD_CTRL_PORT &= ~(1<<LCD_SCK)
	#define LCD_SCK_HIGH()	LCD_CTRL_PORT |= (1<<LCD_SCK)

	#define LCD_SIO_LOW()	LCD_CTRL_PORT &= ~(1<<LCD_SIO)
	#define LCD_SIO_HIGH()	LCD_CTRL_PORT |= (1<<LCD_SIO)
	#define LCD_SIO_GET()	((LCD_CTRL_PIN>>LCD_SIO)&0x01)

	#ifdef LCD_RST
	#define LCD_RST_LOW()	LCD_CTRL_PORT &= ~(1<<LCD_RST)
	#define LCD_RST_HIGH()	LCD_CTRL_PORT |= (1<<LCD_RST)
	#endif	//#ifdef LCD_RST

	#ifdef LCD_PSB
	#define LCD_PSB_LOW()	LCD_CTRL_PORT &= ~(1<<LCD_PSB)
	#define LCD_PSB_HIGH()	LCD_CTRL_PORT |= (1<<LCD_PSB)
	#endif	//#ifdef LCD_PSB


#endif	//#if PARALLEL == 1

//�Ѿ���������
#define LCD_X_MAX	127
#define LCD_Y_MAX	63



//ÿ�е��׵�ַ,����RT12864-4M��Һ��
#define LINE_ONE_ADDRESS   0x80
#define LINE_TWO_ADDRESS   0x90
#define LINE_THREE_ADDRESS 0x88
#define LINE_FOUR_ADDRESS  0x98

//����ָ�Ԥ����
#define LCD_DATA               	1         	//����λ
#define LCD_COMMAND            	0		 	//����λ
#define	LCD_CLEAR_SCREEN       	0x01 	 	//����	
#define	LCD_ADDRESS_RESET      	0x02		//��ַ����	 
#define	LCD_BASIC_FUNCTION	  	0x30		//����ָ�
#define	LCD_EXTEND_FUNCTION	   	0x34		//����ָ�

//��չָ�Ԥ����
#define	LCD_AWAIT_MODE			0x01	 	//����ģʽ	
#define LCD_ROLLADDRESS_ON		0x03		//�������봹ֱ����ַ
#define LCD_IRAMADDRESS_ON		0x02		//��������IRAM��ַ	
#define	LCD_SLEEP_MODE			0x08	 	//����˯��ģʽ
#define	LCD_NO_SLEEP_MODE		0x0c	 	//����˯��ģʽ
#define LCD_GRAPH_ON			0x36		//�򿪻�ͼģʽ
#define LCD_GRAPH_OFF			0x34		//�رջ�ͼģʽ


#ifndef LCD_COLOR
#define LCD_COLOR_BLACK			1
#define LCD_COLOR_WHITE			0
#endif	//#ifndef LCD_COLOR




extern void LCD_Init(void);
extern void LCD_ClearRam(void);
extern void LCD_GraphModeSet(unsigned char Select);
extern void LCD_DisplayStrings_WithAddress(unsigned char ucAdd,const char *p);
extern void LCD_DisplayStrings(unsigned char x,unsigned char y,const char *p);
extern void LCD_DisplayChinese(unsigned char x,unsigned char y,const char *p);
extern void LCD_Point(unsigned char x,unsigned char y,unsigned char color);





#endif	//#ifndef __ST7920_H__
