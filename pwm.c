#include <msp430.h>
#define PWN_PERIRD  100
void PWM_Init() {
    // P2.1 pinini Peripheral (TA1.1) moduna al
    P2DIR |= BIT1;
    P2SEL |= BIT1;
    P2SEL2 &= ~BIT1;

    // Timer_A1 Ayarları
    TA1CCR0 = PWN_PERIRD;             // PWM Periyodu (Maksimum değer)
    TA1CCTL1 = OUTMOD_7;        // Reset/Set modu (Pozitif PWM)
    TA1CCR1 = 0;                // Başlangıç Duty Cycle (%0 - LED sönük)
    
    // TASSEL_2: SMCLK, MC_1: Up Mode (CCR0'a kadar sayar)
    TA1CTL = TASSEL_2 + MC_1 + TACLR;
}

void PWM_SetDuty(unsigned int duty) {
    if (duty > PWN_PERIRD) duty = PWN_PERIRD; // Sınırı aşma
    TA1CCR1 = duty;               // Parlaklığı ayarla
}