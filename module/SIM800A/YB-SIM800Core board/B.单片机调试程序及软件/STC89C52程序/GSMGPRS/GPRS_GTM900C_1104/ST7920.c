

#include "ST7920.h"


#if PARALLEL != 1

void LCD_SendByte(unsigned char a)
{
	unsigned char i,d;
	LCD_CS_HIGH();
	LCD_SIO_OUT();
	for(i=0;i<8;i++)
	{
		LCD_SCK_LOW();  //clrbit(LCD_CTRL,E);  
		d = a&0x80;
		if(d)
			LCD_SIO_HIGH();	//setbit(LCD_CTRL,RW);
		else
			LCD_SIO_LOW();	//clrbit(LCD_CTRL,RW);
		a<<=1;
		LCD_SCK_HIGH();  //setbit(LCD_CTRL,E); //�����ҷ���
	}
	LCD_CS_LOW();
}


#endif



//*******************************************
//*��������:void LCD_CheckBusy(void)	*
//*��������:���з�ʽ��æ					*
//*��ʽ����:��								*
//*�β�˵��:��								*
//*���ز���:��								*
//*ʹ��˵��:��								*
//*******************************************
void LCD_CheckBusy(void)
{
#if PARALLEL == 1
	unsigned char temp;
	LCD_DATA_IN();	//P1 = 0xff;	//����ǰ��1
	LCD_RS_LOW();	//RS = 0;		//ָ��
	LCD_RW_HIGH();	//RW = 1;		//��ģʽ
	_delay_us(5);	//��ʱ
	LCD_E_HIGH();	//E  = 1;	 //ʹ��
	do
	{	
		temp = LCD_DATA_GET();	//temp = BUSY;
	}
  	while( 0x80==(temp&0x80) );	//�ȴ���æ,temp=0x00ʱ��æ
	
	LCD_E_LOW();	//E  = 0;
	//_delay_ms(20);
#else
	unsigned char temp = 0xFC,temp2;
	do
	{
		LCD_SendByte(temp);
		temp = LCD_ReceiveByte();
		temp2 = LCD_ReceiveByte();
		temp &= 0x80;
	}while( temp==0x80 );
#endif	//#if PARALLEL == 1
}


//************************************************************************************
//*��������:void LCD_Write(unsigned char Data_Command , unsigned char uc_Content ) *
//*��������:����ģʽ��LCD�������ݻ�ָ��												 *
//*��ʽ����:unsigned char Data_Command , unsigned char uc_Content					 *
//*�β�˵��:���ݻ�ָ��ı�־λ,ָ������ݵ�����										 *
//*���ز���:��																		 *
//*ʹ��˵��:��																		 *
//************************************************************************************
void LCD_Write( unsigned char Data_Command , unsigned char uc_Content )
{ 
#if PARALLEL == 1

	LCD_CheckBusy();
	LCD_DATA_OUT();

	if(Data_Command)
	{
		LCD_RS_HIGH();	//RS = 1;   //����
		//LCD_RW_LOW();	//RW = 0;   //дģʽ
	}
	else 
	{
		LCD_RS_LOW();	//RS = 0;	//ָ��
		//LCD_RW_LOW();	//RW = 0;   //дģʽ
	}
	LCD_RW_LOW();		//RW = 0;   //дģʽ

  	LCD_DATA_PORT = uc_Content;//���ݷŵ�P1����
  	LCD_E_HIGH();	//E = 1;
	_delay_us(1);	//��ʱ
  	//_nop_();//����Ҫ 
	//_nop_();
   	//_nop_();
    LCD_E_LOW();	//E = 0;		
#else
	unsigned char temp;

	LCD_CheckBusy();
	
	if(Data_Command)
	{
		temp = 0xFA;	//RS = 1,����; RW = 0,дģʽ
	}
	else 
	{
		temp = 0xF8;	//RS = 0,ָ��; RW = 0,дģʽ
	}
	LCD_SendByte(temp);

	temp = uc_Content&0xF0;
	LCD_SendByte(temp);

	temp = (uc_Content<<4)&0xF0;
	LCD_SendByte(temp);
#endif	//#if PARALLEL == 1												
}


//********************************************
//*��������:LCD_ReadData		 *
//*��������:���ж�LCD����					 *
//*��ʽ����:��								 *
//*�β�˵��:��								 *
//*���ز���:���ص�����						 *
//*ʹ��˵��:��								 *
//********************************************
unsigned char LCD_ReadData(void)
{
#if PARALLEL == 1
 	unsigned char uc_Content;
	LCD_CheckBusy();
	//_delay_us(2);

	LCD_DATA_IN();	//P1 = 0xff;//����ǰ��1
    LCD_RS_HIGH();	//RS = 1;   //����
    LCD_RW_HIGH();	//RW = 1;	//��ģʽ

  	LCD_E_HIGH();	//E = 1;	//ʹ��
	_delay_us(1);	//��ʱ����Ҫ 
  	uc_Content = LCD_DATA_GET();	//uc_Content = P1;    //P1�ڵ����ݷŵ�������
  	LCD_E_LOW();	//E = 0;

	_delay_us(1);	//��ʱ����Ҫ 

	return uc_Content;
#endif	//#if PARALLEL == 1
}





//*************************************************************
//*��������:void LCD_DDRAM_AddressSet(unsigned char ucDDramAdd) *
//*��������:�趨DDRAM(�ı���)��ַ����ַ������AC				  *
//*��ʽ����:unsigned char ucDDramAdd								  *
//*�в�˵��:�����ַ										  *
//*��ʽ˵��:												  *
//*		RS	RW	DB7	DB6	DB5	DB4	DB3	DB2	DB1	DB0				  *
//*		0	0	1	AC6	AC5	AC4	AC3	AC2	AC1	AC0				  *
//*���ز���:��												  *
//*ʹ��˵��:												  *
//*	��һ�е�ַ:80H~8FH										  *
//*	�ڶ��е�ַ:90H~9FH										  *
//*	�����е�ַ:A0H~AFH										  *
//*	�����е�ַ:B0H~BFH										  *
//*************************************************************
void LCD_DDRAM_AddressSet(unsigned char ucDDramAdd)
{
	LCD_Write(LCD_COMMAND,LCD_BASIC_FUNCTION);			//����ָ�
	LCD_Write(LCD_COMMAND,ucDDramAdd);				//�趨DDRAM��ַ����ַ������AC
}


//*************************************************************
//*��������:void LCD_CGRAM_AddressSet(unsigned char ucCGramAdd) *
//*��������:�趨CGRAM(�Զ����ֿ���)��ַ����ַ������AC		  *
//*��ʽ����:unsigned char ucCGramAdd								  *
//*�β�˵��:�����ַ										  *
//*��ʽ˵��:												  *
//*		RS	RW	DB7	DB6	DB5	DB4	DB3	DB2	DB1	DB0				  *
//*		0	0	0	1	AC5	AC4	AC3	AC2	AC1	AC0				  *
//*���ز���:��												  *
//*ʹ��˵��:��ַ��ΧΪ40H~3FH								  *
//*************************************************************
void LCD_CGRAM_AddressSet(unsigned char ucCGramAdd)
{
	LCD_Write(LCD_COMMAND,LCD_BASIC_FUNCTION);			//����ָ�
	LCD_Write(LCD_COMMAND,ucCGramAdd);				//�趨CGRAM��ַ����ַ������AC
}


//*******************************************************************************
//*��������:void LCD_GDRAM_AddressSet(unsigned char ucGDramAdd)                   *
//*��������:�趨GDRAM(ͼ����)��ַ����ַ������AC                                 *
//*��ʽ����:unsigned char ucGDramAdd                                                    *
//*�β�˵��:�����ֵַ                                                          *
//*		RS	RW	DB7	DB6	DB5	DB4	DB3	DB2	DB1	DB0                                 *
//*		0	0	1	AC6	AC5	AC4	AC3	AC2	AC1	AC0                                 *
//*		���趨��ֱλ�����趨ˮƽλ��(����д�������ֽ���ɴ�ֱ��ˮƽλ�õ�����)  *	
//*		��ֱ��ַ��Χ:AC6~AC0                                                    *
//*		ˮƽ��ַ��Χ:AC3~AC0                                                    *
//*���ز���:��                                                                  *
//*ʹ��˵��:��������չָ��������ʹ��                                        *
//*******************************************************************************
void LCD_GDRAM_AddressSet(unsigned char ucGDramAdd)
{
	LCD_Write(LCD_COMMAND,LCD_EXTEND_FUNCTION);			//��չָ�
	LCD_Write(LCD_COMMAND,ucGDramAdd);		
}



//*******************************************
//*��������:void LCD_Init(void)	*
//*��������:���г�ʼ��LCD					*
//*��ʽ����:��								*
//*�β�˵��:��								*
//*���ز���:��								*
//*ʹ��˵��:��								*
//*******************************************
void LCD_Init(void)
{
	LCD_CTRL_OUT();
#if PARALLEL == 1
	#ifdef LCD_RST
		LCD_RST_LOW();
		_delay_ms(10);
	  	LCD_RST_HIGH();	//RST = 1;					//��λ�����ߣ�ֹͣ��λ
	#endif
	#ifdef LCD_PSB
	  	LCD_PSB_HIGH();	//PSB = 1;					//ѡ���д���ģʽ	
	#endif
#else
	#ifdef LCD_RST
		LCD_RST_LOW();
		_delay_ms(100);
	  	LCD_RST_HIGH();	//RST = 1;					//��λ�����ߣ�ֹͣ��λ
	#endif
	#ifdef LCD_PSB
	  	LCD_PSB_LOW();	//PSB = 0;					//ѡ�񴮲��д���ģʽ	
	#endif
#endif	//#if PARALLEL == 1


  	LCD_Write(LCD_COMMAND,LCD_BASIC_FUNCTION); //����ָ���   
  	LCD_Write(LCD_COMMAND,LCD_CLEAR_SCREEN);  	//��������ַָ��ָ��00H
  	LCD_Write(LCD_COMMAND,0x06);  		    //�����ƶ�����
  	LCD_Write(LCD_COMMAND,0x0C);  			//����ʾ�����α�

}



//********************************************************
//*��������:void LCD_ClearRam(void)	             *
//*��������:������������					             *
//*��ʽ����:��								             *
//*�β�˵��:��							     	         *
//*���ز���:��								             *
//*ʹ��˵��:DDRAM����20H,���趨DDRAM AC��00H	         *
//*��ʽ˵��:											 *
//*		    RS	RW	DB7	 DB6  DB5  DB4	DB3	DB2	DB1	DB0	 *
//*		    0	0	 0	  0	   0	0	 0   0	 0	 1	 *
//********************************************************
void LCD_ClearRam(void)
{
	LCD_Write(LCD_COMMAND,LCD_BASIC_FUNCTION);			//����ָ�
	LCD_Write(LCD_COMMAND,LCD_CLEAR_SCREEN);			//����
}


//*****************************************************
//*��������:void LCD_GraphModeSet(unsigned char bSelect) *
//*��������:�򿪻�رջ�ͼ��ʾ						  *
//*��ʽ����:unsigned char Select					  *
//*�β�˵��:�򿪻�رջ�ͼ��ʾ�ı�־λ,1��,0�ر�	  *
//*��ʽ˵��:										  *
//*		RS	RW	DB7	DB6	DB5	DB4	DB3	DB2	DB1	DB0		  *
//*		0	0	0	0	1	1	X	RE	 G	 X		  *
//*		                    						  *
//*		RE:0,����ָ�								  *
//*		RE:1,����ָ�								  *
//*		G :0,��ͼ��ʾOFF							  *
//*		G :1,��ͼ��ʾON								  *
//*���ز���:��										  *
//*ʹ��˵��:��										  *
//*****************************************************
void LCD_GraphModeSet(unsigned char Select)
{
	LCD_Write(LCD_COMMAND,LCD_EXTEND_FUNCTION);	//��չָ�
	if(Select)
	{
		LCD_Write(LCD_COMMAND,LCD_GRAPH_ON);		//�򿪻�ͼģʽ
	}
	else
	{
		LCD_Write(LCD_COMMAND,LCD_GRAPH_OFF);		//�رջ�ͼģʽ	
	}
}


//*********************************************************************************
//*��������:void LCD_DisplayStrings_WithAddress(unsigned char ucAdd,unsigned char code *p)	  *
//*��������:��(�ı���)ucAddָ����λ����ʾһ���ַ�(���Ǻ��ֻ���ASCII�������߻��)  *
//*��ʽ����:unsigned char ucAdd,unsigned char code *p							  *
//*�β�˵��:ָ����λ��,Ҫ��ʾ���ַ���											  *
//*��ַ������:80H~87H,90H~97H,88H~8FH,98H~8FH									  *
//*���ز���:��																	  *
//*ʹ��˵��:ʹ��֮ǰҪ��ʼ��Һ��												  *
//*********************************************************************************
void LCD_DisplayStrings_WithAddress(unsigned char ucAdd,const char *p)
{
	unsigned char length;
	length = strlen(p);
    LCD_Write(LCD_COMMAND,LCD_BASIC_FUNCTION);  			//����ָ���   
	LCD_DDRAM_AddressSet(ucAdd);
	for( ;  length ; length -- )
		LCD_Write(LCD_DATA, *(p++));					
}


//*********************************************************************************
//*��������:void LCD_DisplayChinese(unsigned char x,unsigned char y,unsigned char code *p)	  *
//*��������:��(�ı���)ucAddָ����λ����ʾһ���ַ�(���Ǻ��ֻ���ASCII�������߻��)  *
//*��ʽ����:unsigned char x,unsigned char y,const char *p						  *
//*�β�˵��:ָ����λ��,Ҫ��ʾ���ַ���											  *
//*��ַ������: x<8, y<4			                       						  *
//*���ز���:��																	  *
//*ʹ��˵��:ʹ��֮ǰҪ��ʼ��Һ��												  *
//*********************************************************************************
void LCD_DisplayChinese(unsigned char x,unsigned char y,const char *p)
{
	unsigned char length,addr;

	y &= 0x03;	//y < 4
	x &= 0x07;	//x < 8

	switch( y )
	{
		case 0:
			addr = 0x80;
		break;

		case 1:
			addr = 0x90;
		break;

		case 2:
			addr = 0x88;
		break;

		case 3:
			addr = 0x98;
		break;
	}

	addr += x;

	length = strlen(p);

	LCD_Write(LCD_COMMAND,LCD_BASIC_FUNCTION);  			//����ָ���   
	LCD_DDRAM_AddressSet(addr);

	for( ;  length ; length -- )
		LCD_Write(LCD_DATA, *(p++));
}



//*********************************************************************************
//*��������:void LCD_DisplayStrings(unsigned char x,unsigned char y,unsigned char code *p)	  *
//*��������:��(�ı���)ucAddָ����λ����ʾһ��ASCII                                *
//*��ʽ����:unsigned char x,unsigned char y,const char *p						  *
//*�β�˵��:ָ����λ��,Ҫ��ʾ���ַ���											  *
//*��ַ������: x<16, y<4			                       						  *
//*���ز���:��																	  *
//*ʹ��˵��:ʹ��֮ǰҪ��ʼ��Һ��												  *
//*********************************************************************************
void LCD_DisplayStrings(unsigned char x,unsigned char y,const char *p)
{
	unsigned char length,addr;
	unsigned char AddrEO;	//x��ַ����ż
	unsigned char Data1;

	y &= 0x03;	//y < 4
	x &= 0x0F;	//x < 16

	switch( y )
	{
		case 0:
			addr = 0x80;
		break;

		case 1:
			addr = 0x90;
		break;

		case 2:
			addr = 0x88;
		break;

		case 3:
			addr = 0x98;
		break;
	}

	addr += x/2;
	AddrEO = x&0x01;

	length = strlen(p);

	LCD_Write(LCD_COMMAND,LCD_BASIC_FUNCTION);  			//����ָ���   
	LCD_DDRAM_AddressSet(addr);

	if(AddrEO == 0x01)
		{
		Data1 = LCD_ReadData();
		LCD_DDRAM_AddressSet(addr);
		LCD_Write(LCD_DATA, Data1);
		}

	for( ;  length ; length -- )
		LCD_Write(LCD_DATA, *(p++));
}


//*********************************************************************
//*��������:void LCD_ImgDisplay(unsigned char code *img)         *
//*��������:ȫ����ʾ128*64�����ص�ͼ��                                *
//*��ʽ����:unsigned char code *img                                   *
//*�β�˵��:Ҫ��ʾ��ͼ��                                              *
//*Һ��������˵��:                                                    *
//*      ________________128������______________________              *
//*      |(0,0)                                    (7,0)|             *
//*      |                                              |             *
//*   64 |                                              |             *
//*   �� |(0,31)                                  (7,31)|             *
//*   �� |(8,0)                                   (15,0)|             *
//*   �� |                                              |             *
//*      |                                              |             *
//*      |(8,31)                                 (15,31)|             *
//*      |______________________________________________|             *
//*                                                                   *
//���ز���:��                                                         *
//ʹ��˵��:�˺���������CM12864-12��Һ��                               *
//*********************************************************************
void LCD_ImgDisplay(unsigned char *img)
{
  	unsigned char i,j;
    LCD_GraphModeSet(0x00); //�ȹر�ͼ����ʾ����
	for(j=0;j<32;j++)
  	 {
    	for(i=0;i<8;i++)
     	 {
      		LCD_Write(LCD_COMMAND,0x80+j);		//�趨��ֱ����
      		LCD_Write(LCD_COMMAND,0x80+i);		//�趨ˮƽ����
      	    LCD_Write(LCD_DATA,img[j*16+i*2]);	//�������ݸ��ֽ�
      		LCD_Write(LCD_DATA,img[j*16+i*2+1]);	//�������ݵ��ֽ�
    	 }
  	 }
  	for(j=32;j<64;j++)
  	 {
    	for(i=0;i<8;i++)
    	 {
      		LCD_Write(LCD_COMMAND,0x80+j-32);
      		LCD_Write(LCD_COMMAND,0x88+i);
      		LCD_Write(LCD_DATA,img[j*16+i*2]);
     	    LCD_Write(LCD_DATA,img[j*16+i*2+1]);
    	 }
  	 }
	LCD_GraphModeSet(0x01);//����ͼ����ʾ����
}

void LCD_DisplayCLR()
{
  	unsigned char i,j;
    LCD_GraphModeSet(0x00); //�ȹر�ͼ����ʾ����
	for(j=0;j<32;j++)
  	 {
    	for(i=0;i<8;i++)
     	 {
      		LCD_Write(LCD_COMMAND,0x80+j);		//�趨��ֱ����
      		LCD_Write(LCD_COMMAND,0x80+i);		//�趨ˮƽ����
      	    LCD_Write(LCD_DATA,0x00);	//�������ݸ��ֽ�
      		LCD_Write(LCD_DATA,0x00);	//�������ݵ��ֽ�
    	 }
  	 }
  	for(j=32;j<64;j++)
  	 {
    	for(i=0;i<8;i++)
    	 {
      		LCD_Write(LCD_COMMAND,0x80+j-32);
      		LCD_Write(LCD_COMMAND,0x88+i);
      		LCD_Write(LCD_DATA,0x00);
     	    LCD_Write(LCD_DATA,0x00);
    	 }
  	 }
	LCD_GraphModeSet(0x01);//����ͼ����ʾ����
}


//******************************************************************************
//*��������:void LCD_ImgDisplayCharacter(unsigned char x,unsigned char y,unsigned char code *img) *
//*��������:ʹ�û�ͼ�ķ���,��(x,y)����һ��16*16�����ͼ��,Ҳ�������ַ�		   *
//*��ʽ����:unsigned char x,unsigned char y,unsigned char code *img									   *
//*			xȡֵ��Χ:0~7													   *
//*			yȡֵ��Χ:0~31 (���CM12864-12��Һ��)						       *
//*�β�˵��:����ˮƽλ��,���괹ֱλ��,Ҫ��ʾ��ͼ��							   *
//*Һ��������˵��:															   *
//*      ________________128������______________________              *
//*      |(0,0)                                    (7,0)|             *
//*      |                                              |             *
//*   64 |                                              |             *
//*   �� |(0,31)                                  (7,31)|             *
//*   �� |(8,0)                                   (15,0)|             *
//*   �� |                                              |             *
//*      |                                              |             *
//*      |(8,31)                                 (15,31)|             *
//*      |______________________________________________|             *
//*                                                                   *
//*���ز���:��                                                              *
//*ʹ��˵��:�˺���������CM12864-12��Һ��			*
//******************************************************************************
void LCD_ImgDisplayCharacter(unsigned char x,unsigned char y,unsigned char *img)
{
  	unsigned char i;
 	LCD_GraphModeSet(0x00);		//�ȹر�ͼ����ʾ����
    LCD_Write(LCD_COMMAND,LCD_EXTEND_FUNCTION);
	for(i=0;i<16;i++)
  	 {
		LCD_GDRAM_AddressSet(0x80+y+i);
		LCD_GDRAM_AddressSet(0x80+x);
      	LCD_Write(LCD_DATA,img[i*2]);
      	LCD_Write(LCD_DATA,img[i*2+1]);
  	 }
   LCD_GraphModeSet(0x01);				//����ͼ����ʾ����
}



//******************************************************************************
//*��������:void LCD_Point(unsigned char x,unsigned char y,unsigned char color)*
//*��������:ʹ�û�ͼ�ķ���,��(x,y)����һ��16*16�����ͼ��,Ҳ�������ַ�		   *
//*��ʽ����:unsigned char x,unsigned char y,unsigned char color                *
//*			xȡֵ��Χ:0~127													   *
//*			yȡֵ��Χ:0~63 (���CM12864-12��Һ��)						       *
//*�β�˵��:����ˮƽλ��,���괹ֱλ��           							   *
//*Һ��������˵��:															   *
//*      ________________128������______________________              *
//*      |(0,0)                                    (7,0)|             *
//*      |                                              |             *
//*   64 |                                              |             *
//*   �� |(0,31)                                  (7,31)|             *
//*   �� |(8,0)                                   (15,0)|             *
//*   �� |                                              |             *
//*      |                                              |             *
//*      |(8,31)                                 (15,31)|             *
//*      |______________________________________________|             *
//*                                                                   *
//*���ز���:��                                                              *
//*ʹ��˵��:�˺���������CM12864-12��Һ��			*
//******************************************************************************
void LCD_Point(unsigned char x,unsigned char y,unsigned char color)
{
	unsigned char i,AddrX,AddrY;	//�Ĵ�����ַ
	unsigned char BitTemp;		//�õ���16bit��λ��
	unsigned char DataH,DataL;

	AddrX = x>>4;				// x/16
	AddrY = y&0x3F;				// y%64
	if(y > 31)
	{
		AddrX += 8;
		AddrY -= 32;
	}
	BitTemp = x&0x0F;			// x%16

	
 	LCD_GraphModeSet(0x00);		//�ȹر�ͼ����ʾ����
    //LCD_Write(LCD_COMMAND,LCD_EXTEND_FUNCTION);
	
	for(i=0;i<4;i++)
	{
	LCD_GDRAM_AddressSet(0x80+AddrY);
	LCD_GDRAM_AddressSet(0x80+AddrX);
	//LCD_Write(LCD_COMMAND,LCD_BASIC_FUNCTION);			//����ָ�
	DataH = LCD_ReadData();
	DataH = LCD_ReadData();
	DataL = LCD_ReadData();
	//DataH = LCD_ReadData();
	//DataL = LCD_ReadData();
	}

	if(color == LCD_COLOR_BLACK)
	{
		if(BitTemp > 7)
		{
			DataL |= (0x80>>(BitTemp-8));
		}
		else
		{
			DataH |= (0x80>>(BitTemp));
		}
	}
	else
	{
	}

	LCD_GDRAM_AddressSet(0x80+AddrY);
	LCD_GDRAM_AddressSet(0x80+AddrX);
	LCD_Write(LCD_DATA,DataH);
	LCD_Write(LCD_DATA,DataL);
	
	LCD_GraphModeSet(0x01);				//����ͼ����ʾ����
}



