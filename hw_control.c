#include "hw_control.h"
#include <msp430.h>
#include <stdint.h>
#include "app_config.h"

unsigned int LDR_ReadRaw(void)
{
    ADC10CTL0 |= ENC | ADC10SC;
    while (ADC10CTL1 & ADC10BUSY) {
        /* Conversion completes in hardware. */
    }
    return ADC10MEM;
}

unsigned int LDR_CalculatePercentage(unsigned int rawADC)
{
    static uint32_t filtered_percent = 0;
    static bool filter_initialized = false;
    uint32_t light_percent;

    if (rawADC < LDR_DARK_RAW) {
        rawADC = LDR_DARK_RAW;
    }
    if (rawADC > LDR_BRIGHT_RAW) {
        rawADC = LDR_BRIGHT_RAW;
    }

    light_percent = ((uint32_t)(rawADC - LDR_DARK_RAW) * 100U) /
                    (LDR_BRIGHT_RAW - LDR_DARK_RAW);

    if (!filter_initialized) {
        filtered_percent = light_percent;
        filter_initialized = true;
    } else {
        /* 1/8 low-pass filter reduces visible PWM and OLED jitter. */
        filtered_percent = ((filtered_percent * 7U) + light_percent) >> 3;
    }

    return (unsigned int)filtered_percent;
}

bool PIR_IsMotionDetected(void)
{
    return (P1IN & PIR_PIN) != 0;
}

void PWM_SetDuty(unsigned int duty)
{
    if (duty > PWM_PERIOD_TICKS) {
        duty = PWM_PERIOD_TICKS;
    }
    TA1CCR1 = duty;
}

unsigned int PWM_GetDuty(void)
{
    return TA1CCR1;
}
