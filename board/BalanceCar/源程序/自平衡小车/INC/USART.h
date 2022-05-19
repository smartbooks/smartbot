#ifndef __USART_H_
#define	__USART_H_

#include "stm32f10x.h"
#include <stdio.h>

#define USART1_DR_Base  0x40013804

void USART_NVIC_Configuration(void);
void USART_Config(void);

int fputc(int ch, FILE *f);
void USART_printf(USART_TypeDef* USARTx, uint8_t *Data,...);	   
static char *itoa(int32_t value, char *string, int radix);

#endif /* __USART1_H */
