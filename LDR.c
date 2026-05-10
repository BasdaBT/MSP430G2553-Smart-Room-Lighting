#include <msp430.h>
#include <stdint.h>

void ADC_Init() {
    ADC10CTL0 = ADC10SHT_2 + ADC10ON; // Örnekleme zamanı ve ADC Açık
    ADC10CTL1 = INCH_0;               // P1.0 (A0 kanalı) seçildi
    ADC10AE0 |= BIT0;                 // P1.0 için analog girişi aktif et
}

unsigned int LDR_Oku() {
    ADC10CTL0 |= ENC + ADC10SC;
    while (ADC10CTL0 & ADC10BUSY);    
    return ADC10MEM;                  
}


unsigned int LDR_true(unsigned int adcValue )
{
    float k = 0.1;
    uint32_t min_isik = 467; 
    uint32_t max_isik = 867;
    if (adcValue < min_isik) adcValue = min_isik;
    if (adcValue > max_isik) adcValue = max_isik;

    // Oransal hesaplama (0-100 arası yüzdeye çevir)
    uint32_t yüzde_isik = ((adcValue - min_isik) * 1000) / (max_isik - min_isik);
    float y =  0 ;
    y = k * yüzde_isik + (1.0 - k) * y;
    return y ;
}