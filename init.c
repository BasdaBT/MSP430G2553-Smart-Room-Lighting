#include "init.h"
#include <msp430.h>
#include "app_config.h"

static void Clock_Init(void)
{
    /* Factory calibration constants give UART and I2C reliable timing. */
    if (CALBC1_1MHZ != 0xFF) {
        DCOCTL = 0;
        BCSCTL1 = CALBC1_1MHZ;
        DCOCTL = CALDCO_1MHZ;
    }
}

void MCU_Init(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    Clock_Init();

    /* LDR: P1.0 / ADC10 channel A0. */
    ADC10CTL0 = ADC10SHT_2 | ADC10ON;
    ADC10CTL1 = ADC10SSEL_3 | INCH_0;
    ADC10AE0 |= LDR_PIN;

    /* PIR: P1.5, pull-down, interrupt first on the rising edge. */
    P1SEL &= ~PIR_PIN;
    P1SEL2 &= ~PIR_PIN;
    P1DIR &= ~PIR_PIN;
    P1REN |= PIR_PIN;
    P1OUT &= ~PIR_PIN;
    P1IES &= ~PIR_PIN;
    P1IFG &= ~PIR_PIN;
    P1IE |= PIR_PIN;

    /* LED PWM: P2.1 / Timer1_A capture-compare output 1. */
    P2DIR |= PWM_LED_PIN;
    P2SEL |= PWM_LED_PIN;
    P2SEL2 &= ~PWM_LED_PIN;
    TA1CCR0 = PWM_PERIOD_TICKS;
    TA1CCTL1 = OUTMOD_7;
    TA1CCR1 = 0;
    TA1CTL = TASSEL_2 | MC_1 | TACLR;
}
