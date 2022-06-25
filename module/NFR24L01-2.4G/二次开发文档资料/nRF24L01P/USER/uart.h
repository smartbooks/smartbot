
#ifndef __uart__
#define __uart__

void InitUART();

void OpenUARTReceive();

void CloseUARTReceive();

void SendToUART(unsigned char *text);

void log(unsigned char *text);

#endif
