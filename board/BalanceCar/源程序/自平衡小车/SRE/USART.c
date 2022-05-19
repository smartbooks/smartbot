#include "USART.h"
#include <stdarg.h>


/*
 * 函数名：USART1_Config
 * 描述  ：USART1 GPIO 配置,工作模式配置。115200 8-N-1
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;                                                    //定义一个结构体
	USART_InitTypeDef USART_InitStructure;	                                                //定义一个结构体

/**********************************************************************************************************************
**************************************************串口一，接USB口******************************************************
***********************************************************************************************************************/	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);           //使能串口1时钟
//配置USART1 Tx(PA.09)复用推挽输出	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	                                            //定义PA9为串口1的Tx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    

//配置USART1 Rx(PA.10)浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;		                                    	//定义PA10为串口1的Rx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure); 
	USART_ClearFlag(USART1, USART_FLAG_RXNE);	 //清除中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
	USART_GetFlagStatus(USART1, USART_FLAG_TC);                                       
/**********************************************************************************************************************
**************************************************串口三,接WIFI模块******************************************************
***********************************************************************************************************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);                                   //使能串口2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
//配置USART2 Tx(PA.02)复用推挽输出	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	                                            //定义PA2为串口1的Tx
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);    

//配置USART2 Tx(PA.3)浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;		                                    	//定义PA3为串口1的Rx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	USART_ClearFlag(USART3, USART_FLAG_RXNE);	 //清除中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART3, ENABLE);
	USART_GetFlagStatus(USART3, USART_FLAG_TC);                                       
}

/*
 * 函数名：USART_NVIC_Configuration
 * 描述  ：USART中断优先级配置
 * 输入  ：无
 * 输出  ：无	
 */
void USART_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	
/************************串口1***********************************/														
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
/************************串口2***********************************/
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

} 

/*
 * 函数名：fputc
 * 描述  ：重定向c库函数printf到USART1
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由printf调用
 */
int fputc(int ch, FILE *f)
{
	/* 将Printf内容发往串口 */
	USART_SendData(USART1, (unsigned char) ch);
//	while (!(USART1->SR & USART_FLAG_TXE));
	while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);	
	return (ch);
}

/*
 * 函数名：USART2_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，这里只用到了串口2，即USART2
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  ：无
 * 返回  ：无 
 * 调用  ：外部调用
 *         典型应用  USART2_printf( USART2, "\r\n this is a demo \r\n" );
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

  while ( *Data != 0)     // 判断是否到达字符串结束符
  {				                          
	if ( *Data == 0x5c )  //'\'
	{									  
		switch ( *++Data )
		{
			case 'r':							          //回车符
				USART_SendData(USARTx, 0x0d);
				Data ++;
				break;

			case 'n':							          //换行符
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
			case 's':							     	    //字符串
				s = va_arg(ap, const char *);
                for ( ; *s; s++) 
				{
					USART_SendData(USARTx,*s);
					while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
                }
				Data++;
            break;

            case 'd':										//十进制
               d = va_arg(ap, int32_t);
               itoa(d, buf, 10);                            //将十进制数变成字符串
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
 * 函数名：itoa
 * 描述  ：将整形数据转换成字符串
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -buf 转换后的字符串
 *         -radix = 10
 * 输出  ：无
 * 返回  ：无
 * 调用  ：被USART2_printf()调用
 */	
static char *itoa(int32_t value, char *string, int radix)
{
    int32_t     i, d;
    int     flag = 0;
    char    *ptr = string;                    //定义一个指针变量，将字符串地址

 
    if (radix != 10)                          //如果输出的值不是十进制数，那么字值串为 
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
  
    if (value < 0)							 //如果要转换的值是负值，那么输出有字符串前面先加上一个负，然后将原来的值乘上“-1”变成正值
    {
        *ptr++ = '-';
        value *= -1;
    }

    for (i = 1000000000; i > 0; i /= 10)
    {
        d = value / i;                       //进行循环取整的工作，例如第一次是取10000的整数，第二次是1000，第三次100，第四次10，第五次为1

        if (d || flag)                       //flag初值为0，作用是用来不显示无效的0，如待转换的数为123，那么前两个的循环就不会将前两次0记录下来
        {
            *ptr++ = (char)(d + 0x30);       //将数字转成ASCII码
            value -= (d * i);			     //将原有的数值减去取整的整数值，例如待转换的数为12356，那么第一次循环后value=12356-1*10000=2356
            flag = 1;                        //如果有非零值出现，那后面出来的0都需要记录下来
        }
    }

    *ptr = 0;

    return string;

} 
