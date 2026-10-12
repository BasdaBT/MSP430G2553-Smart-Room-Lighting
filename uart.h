// uart.h
#ifndef UART_H_
#define UART_H_

#include <stdbool.h>

void UART_Init(void);
void UART_PrintChar(char c);
void UART_PrintText(const char *text);
bool UART_IsReceiveReady(void);
char UART_ReceiveChar(void);

#endif
