#include <msp430.h>

#define TX_PIN BIT1  // P1.1 artık TX (Gönderme)
#define RX_PIN BIT2  // P1.2 artık RX (Alma)

// 1 MHz saat hızı ve 9600 Baud Rate için bekleme süreleri
// 1000000 / 9600 = ~104 cycle
#define BIT_TIME 104 
#define HALF_BIT_TIME 52

// Senin orijinal fonksiyon isminle:
void UART_Init() {
    // TX Pin Ayarı (P1.1)
    P1DIR |= TX_PIN;     // Çıkış olarak ayarla
    P1OUT |= TX_PIN;     // UART hattı boşta (idle) iken her zaman HIGH(1) olmalıdır

    // RX Pin Ayarı (P1.2)
    P1DIR &= ~RX_PIN;    // Giriş olarak ayarla
}

// Senin orijinal fonksiyon isminle:
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

// Senin orijinal fonksiyon isminle:
void UART_PrintText(char *text) {
    while (*text) {
        UART_PrintChar(*text++);
    }
}

// Gelen veriyi okumak istersen diye (Bunu yeni ekledim)
char UART_ReceiveChar() {
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