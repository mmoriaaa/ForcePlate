#ifndef __UART_H_
#define __UART_H_

void SendData(unsigned char ch);
void SendString(char *s);
void UartInit(void);
void U2SendString(char *s);
void U2SendData(unsigned char ch);


#endif
