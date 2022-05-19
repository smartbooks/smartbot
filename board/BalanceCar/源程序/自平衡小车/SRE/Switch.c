#include "Switch.h"

void Switch_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    //LED“˝Ω≈≈‰÷√£¨PC08~PC15
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

// if(Switch1==0)
//	   	  LED1=0;
//	  else
//	      LED1=1;
//	  if(Switch2==0)
//	   	  LED2=0;
//	  else
//	      LED2=1;
//	  if(Switch3==0)
//	   	  LED3=0;
//	  else
//	      LED3=1;
//	  if(Switch4==0)
//	   	  LED4=0;
//	  else
//	      LED4=1;

