#include <msp430.h>

void UART_Init() {
    P1SEL |= BIT1 + BIT2;            // P1.1 (RX) ve P1.2 (TX) seçildi
    P1SEL2 |= BIT1 + BIT2;
    
    UCA0CTL1 |= UCSWRST;             // Reset moduna al
    UCA0CTL1 |= UCSSEL_2;            // SMCLK kullan (1MHz)
    UCA0BR0 = 104;                   // 9600 Baud hızı ayarı (1MHz / 9600)
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS0;               // Modülasyon ayarı
    UCA0CTL1 &= ~UCSWRST;            // UART modülünü başlat
}

void UART_PrintChar(char c) {
    while (!(IFG2 & UCA0TXIFG));
    UCA0TXBUF = c;
}

void UART_PrintText(char *text) {
    while (*text) {
        UART_PrintChar(*text++);
    }
}