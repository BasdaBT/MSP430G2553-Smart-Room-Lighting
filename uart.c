// uart.c
#include <msp430.h>
#include "uart.h"

#define TX_PIN BIT1  // P1.1 TX (Gönderme)
#define RX_PIN BIT2  // P1.2 RX (Alma)

// 1 MHz saat hızı ve 9600 Baud Rate için bekleme süreleri
// 1000000 / 9600 = ~104 cycle
#define BIT_TIME 104 
#define HALF_BIT_TIME 52

void UART_PrintChar(char c) {
    // Start Biti (Hattı LOW'a çekiyoruz)
    P1OUT &= ~TX_PIN;
    __delay_cycles(BIT_TIME);

    // 8 Veri Biti 
    int i = 0 ;
    for (; i < 8; i++) {
        if (c & 1) {
            P1OUT |= TX_PIN;  
        } else {
            P1OUT &= ~TX_PIN; 
        }
        c >>= 1;                 
        __delay_cycles(BIT_TIME);
    }

    // Stop Biti (Hattı tekrar HIGH'a çekiyoruz)
    P1OUT |= TX_PIN;
    __delay_cycles(BIT_TIME);
}

void UART_PrintText(char *text) {
    while (*text) {
        UART_PrintChar(*text++);
    }
}

char UART_ReceiveChar(void) {
    char rxData = 0;
    
    // Start bitini bekle
    while (P1IN & RX_PIN); 
    __delay_cycles(HALF_BIT_TIME);  
    
    // 8 Veri Bitini Oku
    int i = 0;
    for (; i < 8; i++) {
        __delay_cycles(BIT_TIME); 
        if (P1IN & RX_PIN) {
            rxData |= (1 << i);   
        }
    }
    
    __delay_cycles(BIT_TIME); 
    return rxData;
}