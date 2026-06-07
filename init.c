// init.c
#include "init.h"
#include <msp430.h>

void MCU_Init(void) {
    WDTCTL = WDTPW | WDTHOLD;

    // LDR Init
    ADC10CTL0 = ADC10SHT_2 + ADC10ON; 
    ADC10CTL1 = INCH_0;               
    ADC10AE0 |= BIT0;                 

    // PIR Kesme (Interrupt) Init - Hoca için şov kısmı :)
    P2DIR &= ~BIT0;
    P2REN |= BIT0;
    P2OUT &= ~BIT0;
    P2IES &= ~BIT0;
    P2IFG &= ~BIT0;
    P2IE  |= BIT0; 

    // PWM Init
    P2DIR |= BIT1;
    P2SEL |= BIT1;              
    P2SEL2 &= ~BIT1;
    TA1CCR0 = 100;              
    TA1CCTL1 = OUTMOD_7;        
    TA1CCR1 = 0;                
    TA1CTL = TASSEL_2 + MC_1 + TACLR; 

    // Yazılımsal UART Init
    P1DIR |= BIT1;              
    P1OUT |= BIT1;              
    P1DIR &= ~BIT2;             
}
