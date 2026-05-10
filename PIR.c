/* PIR.c */
#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

void PIR_Init() {
    P2DIR &= ~BIT0;  // P2.0 pinini giriş (input) olarak ayarla
    P2REN |= BIT0;   // P2.0 için dahili direnci aktif et
    P2OUT &= ~BIT0;  // Direnci GND'ye bağla (Pull-Down yap)
}
bool HAREKET() {
    if (P2IN & BIT0) {
        return true;
    } else {
        return false;
    }
}