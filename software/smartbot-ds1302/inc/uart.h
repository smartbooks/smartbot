
#ifndef __UART_H__
#define __UART_H__

void InitUART();

void OpenUARTReceive();

void CloseUARTReceive();

void SendToUART(unsigned char *text);

void log(unsigned char *text);

#endif
