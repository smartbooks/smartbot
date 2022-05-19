#include "USART.h"
#include <stdarg.h>


/*
 * ��������USART1_Config
 * ����  ��USART1 GPIO ����,����ģʽ���á�115200 8-N-1
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */
void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;                                                    //����һ���ṹ��
	USART_InitTypeDef USART_InitStructure;	                                                //����һ���ṹ��

/**********************************************************************************************************************
**************************************************����һ����USB��******************************************************
***********************************************************************************************************************/	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);           //ʹ�ܴ���1ʱ��
//����USART1 Tx(PA.09)�����������	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	                                            //����PA9Ϊ����1��Tx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    

//����USART1 Rx(PA.10)��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;		                                    	//����PA10Ϊ����1��Rx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure); 
	USART_ClearFlag(USART1, USART_FLAG_RXNE);	 //����ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
	USART_GetFlagStatus(USART1, USART_FLAG_TC);                                       
/**********************************************************************************************************************
**************************************************������,��WIFIģ��******************************************************
***********************************************************************************************************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);                                   //ʹ�ܴ���2ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
//����USART2 Tx(PA.02)�����������	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	                                            //����PA2Ϊ����1��Tx
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);    

//����USART2 Tx(PA.3)��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;		                                    	//����PA3Ϊ����1��Rx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	USART_ClearFlag(USART3, USART_FLAG_RXNE);	 //����ж�
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART3, ENABLE);
	USART_GetFlagStatus(USART3, USART_FLAG_TC);                                       
}

/*
 * ��������USART_NVIC_Configuration
 * ����  ��USART�ж����ȼ�����
 * ����  ����
 * ���  ����	
 */
void USART_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	
/************************����1***********************************/														
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
/************************����2***********************************/
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

} 

/*
 * ��������fputc
 * ����  ���ض���c�⺯��printf��USART1
 * ����  ����
 * ���  ����
 * ����  ����printf����
 */
int fputc(int ch, FILE *f)
{
	/* ��Printf���ݷ������� */
	USART_SendData(USART1, (unsigned char) ch);
//	while (!(USART1->SR & USART_FLAG_TXE));
	while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);	
	return (ch);
}

/*
 * ��������USART2_printf
 * ����  ����ʽ�������������C���е�printf��������û���õ�C��
 * ����  ��-USARTx ����ͨ��������ֻ�õ��˴���2����USART2
 *		     -Data   Ҫ���͵����ڵ����ݵ�ָ��
 *			   -...    ��������
 * ���  ����
 * ����  ���� 
 * ����  ���ⲿ����
 *         ����Ӧ��  USART2_printf( USART2, "\r\n this is a demo \r\n" );
 *            		 USART2_printf( USART2, "\r\n %d \r\n", i );
 *            		 USART2_printf( USART2, "\r\n %s \r\n", j );
 */
void USART_printf(USART_TypeDef* USARTx, uint8_t *Data,...)
{
  const char *s;
  int32_t d;   
  char buf[16];

  va_list ap;
  va_start(ap, Data);

  while ( *Data != 0)     // �ж��Ƿ񵽴��ַ���������
  {				                          
	if ( *Data == 0x5c )  //'\'
	{									  
		switch ( *++Data )
		{
			case 'r':							          //�س���
				USART_SendData(USARTx, 0x0d);
				Data ++;
				break;

			case 'n':							          //���з�
				USART_SendData(USARTx, 0x0a);	
				Data ++;
				break;
				
			default:
				Data ++;
			    break;
		}			 
	}
	else if ( *Data == '%')                                 //
	{									                    //
		switch ( *++Data )
		{				
			case 's':							     	    //�ַ���
				s = va_arg(ap, const char *);
                for ( ; *s; s++) 
				{
					USART_SendData(USARTx,*s);
					while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
                }
				Data++;
            break;

            case 'd':										//ʮ����
               d = va_arg(ap, int32_t);
               itoa(d, buf, 10);                            //��ʮ����������ַ���
               for (s = buf; *s; s++) 
		       {
					USART_SendData(USARTx,*s);
					while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
               }
			   Data++;
            break;

			default:
				Data++;
			break;
		}		 
	} // end of else if
	else 
	USART_SendData(USARTx, *Data++);
	while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
  }
}


/*
 * ��������itoa
 * ����  ������������ת�����ַ���
 * ����  ��-radix =10 ��ʾ10���ƣ��������Ϊ0
 *         -value Ҫת����������
 *         -buf ת������ַ���
 *         -radix = 10
 * ���  ����
 * ����  ����
 * ����  ����USART2_printf()����
 */	
static char *itoa(int32_t value, char *string, int radix)
{
    int32_t     i, d;
    int     flag = 0;
    char    *ptr = string;                    //����һ��ָ����������ַ�����ַ

 
    if (radix != 10)                          //��������ֵ����ʮ����������ô��ֵ��Ϊ 
    {
        *ptr = 0;
        return string;
    }

    if (!value)                   
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }
  
    if (value < 0)							 //���Ҫת����ֵ�Ǹ�ֵ����ô������ַ���ǰ���ȼ���һ������Ȼ��ԭ����ֵ���ϡ�-1�������ֵ
    {
        *ptr++ = '-';
        value *= -1;
    }

    for (i = 1000000000; i > 0; i /= 10)
    {
        d = value / i;                       //����ѭ��ȡ���Ĺ����������һ����ȡ10000���������ڶ�����1000��������100�����Ĵ�10�������Ϊ1

        if (d || flag)                       //flag��ֵΪ0����������������ʾ��Ч��0�����ת������Ϊ123����ôǰ������ѭ���Ͳ��Ὣǰ����0��¼����
        {
            *ptr++ = (char)(d + 0x30);       //������ת��ASCII��
            value -= (d * i);			     //��ԭ�е���ֵ��ȥȡ��������ֵ�������ת������Ϊ12356����ô��һ��ѭ����value=12356-1*10000=2356
            flag = 1;                        //����з���ֵ���֣��Ǻ��������0����Ҫ��¼����
        }
    }

    *ptr = 0;

    return string;

} 
