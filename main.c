#include <msp430.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "i2c.h"
#include "ssd1306.h"

#define LONG_DELAY  __delay_cycles(3000000)
#define SHORT_DELAY __delay_cycles(50000)

char bright[] = "Ortam Aydinlik";
char led_off[] = "Led Kapali";
char hareket_Var[] = "HAREKET VAR   ";
char hareket_Yok[] = "HAREKET YOK   ";
char percent[] = "%";
char tempBuffer[16];

bool hareket = false;

void PIR_Init(void);
bool HAREKET(void);
void control(unsigned int trueADC);
void bluutooth(unsigned int trueADC);

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;                  
    LONG_DELAY;
    
    PIR_Init();            
    ADC_Init();
    i2c_init();
    UART_Init();
    PWM_Init(); 
    ssd1306_init();
    
    ssd1306_clearDisplay();

    LONG_DELAY;
    
    while(1) {
        unsigned int adcValue = LDR_Oku();
        unsigned int trueADC = LDR_true(adcValue);
        hareket = HAREKET();
        
        control(trueADC);
        
        ssd1306_printText(0, 4, percent);
        ssd1306_printUI32(1, 4, (uint32_t)trueADC, HCENTERUL_OFF);
        
        bluutooth(trueADC);
        SHORT_DELAY;
    }
}

void control(unsigned int trueADC) {
    ssd1306_clearDisplay();
    
    if (trueADC >= 50) {
        PWM_SetDuty(0);
        ssd1306_printTextBlock(0, 0, bright);
        ssd1306_printTextBlock(0, 1, led_off);
    } else {
        if (hareket) {
            PWM_SetDuty(100);
            ssd1306_printTextBlock(0, 2, hareket_Var);
        } else {
            PWM_SetDuty(0);
            ssd1306_printTextBlock(0, 2, hareket_Yok);
        }
    }
}

void bluutooth(unsigned int trueADC) {
    ssd1306_ultoa((uint32_t)trueADC, tempBuffer);
    UART_PrintText("LDR Degeri: ");
    UART_PrintText(tempBuffer);
    
    if (hareket) {
        UART_PrintText(" - Hareket VAR\r\n");
    } else {
        UART_PrintText(" - Hareket YOK\r\n");
    }
}
//mspdebug rf2500 "prog Blink LED.out"