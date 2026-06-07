// hw_control.c
#include "hw_control.h"
#include <msp430.h>
#include <stdint.h>

// ==========================================
// LDR FONKSİYONLARI
// ==========================================
unsigned int LDR_ReadRaw(void) {
    ADC10CTL0 |= ENC + ADC10SC;
    while (ADC10CTL0 & ADC10BUSY);    
    return ADC10MEM;                  
}

unsigned int LDR_CalculatePercentage(unsigned int rawADC) {
    const uint32_t min_isik = 1; 
    const uint32_t max_isik = 1004;
    
    if (rawADC < min_isik) rawADC = min_isik;
    if (rawADC > max_isik) rawADC = max_isik;

    uint32_t yuzde_isik = ((rawADC - min_isik) * 100) / (max_isik - min_isik);

    static uint32_t y = 0; 
    
    if (y == 0) {
        y = yuzde_isik;
    } else {
        y = ((y * 7) + yuzde_isik) >> 3; 
    }
    
    return (unsigned int)y;
}

// ==========================================
// PIR FONKSİYONLARI
// ==========================================
bool PIR_IsMotionDetected(void) {
    return (P2IN & BIT0) ? true : false;
}

// ==========================================
// PWM FONKSİYONLARI
// ==========================================
void PWM_SetDuty(unsigned int duty) {
    if (duty > PWM_PERIOD) duty = PWM_PERIOD; 
    TA1CCR1 = duty;               
}