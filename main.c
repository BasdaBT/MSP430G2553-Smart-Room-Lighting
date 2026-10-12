#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#include "init.h"
#include "hw_control.h"
#include "uart.h"
#include "i2c.h"
#include "ssd1306.h"
#include "app_config.h"

char tempBuffer[16];

volatile bool g_motion_detected = false;

typedef enum {
    STATE_DAYLIGHT,
    STATE_NIGHT_IDLE,
    STATE_NIGHT_ACTIVE
} SystemState_t;

SystemState_t current_state = STATE_DAYLIGHT;
SystemState_t previous_state = STATE_DAYLIGHT;
bool g_first_run = true;

// Fonksiyon Prototipleri
void System_BootSequence(void);
void System_UpdateState(unsigned int lightPercent);
void UI_UpdateOLED(unsigned int lightPercent);
void Comms_SendBluetooth(unsigned int lightPercent);

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;

    MCU_Init();
    UART_Init();
    i2c_init();
    g_motion_detected = PIR_IsMotionDetected();
    ssd1306_init();

    ssd1306_clearDisplay();
    __delay_cycles(BOOT_STEP_DELAY_CYCLES);

    System_BootSequence();
    __bis_SR_register(GIE);

    unsigned int telemetryCounter = TELEMETRY_PERIOD_LOOPS;

    while(1) {

        unsigned int rawADC = LDR_ReadRaw();
        unsigned int lightPercent = LDR_CalculatePercentage(rawADC);

        System_UpdateState(lightPercent);

        if ((current_state != previous_state) || g_first_run ||
            (++telemetryCounter >= TELEMETRY_PERIOD_LOOPS)) {
            Comms_SendBluetooth(lightPercent);
            telemetryCounter = 0;
        }

        UI_UpdateOLED(lightPercent);

        __delay_cycles(MAIN_LOOP_DELAY_CYCLES);
    }
}

void System_BootSequence(void) {
    ssd1306_printTextBlock(15, 2, ">>> SMART SYSTEM <<<");
    ssd1306_printTextBlock(30, 4, "INITIATING");

    int i = 0;
    for (i = 0; i < 4; i++) {
        ssd1306_printText(45 + (i * 10), 6, ".");
        __delay_cycles(BOOT_STEP_DELAY_CYCLES);
    }

    ssd1306_clearDisplay();
    __delay_cycles(BOOT_STEP_DELAY_CYCLES);
}

void System_UpdateState(unsigned int lightPercent) {
    if (lightPercent >= LIGHT_THRESHOLD_PERCENT) {
        current_state = STATE_DAYLIGHT;
        PWM_SetDuty(0);
    } else {
        if (g_motion_detected) {
            current_state = STATE_NIGHT_ACTIVE;
            PWM_SetDuty(100 - lightPercent);
        } else {
            current_state = STATE_NIGHT_IDLE;
            PWM_SetDuty(0);
        }
    }
}

void UI_UpdateOLED(unsigned int lightPercent) {
    if ((current_state != previous_state) || g_first_run) {
        switch(current_state) {
            case STATE_DAYLIGHT:
                ssd1306_printText(0, 0, "[*] MOD: GUNDUZ   ");
                ssd1306_printText(0, 2, "[ ] LED: KAPALI   ");
                ssd1306_printText(0, 4, "( ) HRKT: YOK     ");
                break;
            case STATE_NIGHT_IDLE:
                ssd1306_printText(0, 0, "[~] MOD: GECE     ");
                ssd1306_printText(0, 2, "[ ] LED: KAPALI   ");
                ssd1306_printText(0, 4, "( ) HRKT: BEKLIYOR");
                break;
            case STATE_NIGHT_ACTIVE:
                ssd1306_printText(0, 0, "[~] MOD: GECE     ");
                ssd1306_printText(0, 2, "[#] LED: YANIYOR! ");
                ssd1306_printText(0, 4, "(!) HRKT: ALARM!  ");
                break;
        }
        previous_state = current_state;
        g_first_run = false;
    }

    char light_bar[13] = "[          ]";
    uint8_t bar_count = lightPercent / 10;

    int i = 0;
    for(i = 0; i < 10; i++) {
        if(i < bar_count) {
            light_bar[i+1] = '=';
        }
    }
    ssd1306_printText(0, 6, "ISIK: %");
    ssd1306_printUI32(45, 6, (uint32_t)lightPercent, HCENTERUL_OFF);

    if(lightPercent < 10) ssd1306_printText(51, 6, "  ");
    else if (lightPercent < 100) ssd1306_printText(57, 6, " ");

    ssd1306_printText(0, 7, light_bar);

    static char hb_chars[] = {'|', '/', '-', '\\'};
    static uint8_t hb_idx = 0;
    char hb_str[2] = {hb_chars[hb_idx], '\0'};

    ssd1306_printText(115, 0, hb_str);
    hb_idx++;
    if(hb_idx > 3) hb_idx = 0;
}

void Comms_SendBluetooth(unsigned int lightPercent) {
    UART_PrintText("\r\n+---[ SISTEM TELEMETRI ]---+\r\n");

    UART_PrintText("| ISIK SEVIYESI : %");
    ssd1306_ultoa((uint32_t)lightPercent, tempBuffer);
    UART_PrintText(tempBuffer);
    UART_PrintText("\r\n");

    UART_PrintText("| SISTEM DURUMU : ");
    switch(current_state) {
        case STATE_DAYLIGHT:
            UART_PrintText("GUNDUZ [*]\r\n");
            break;
        case STATE_NIGHT_IDLE:
            UART_PrintText("GECE BEKLEME [-]\r\n");
            break;
        case STATE_NIGHT_ACTIVE:
            UART_PrintText("HAREKET ALARMI [!]\r\n");
            break;
    }
    UART_PrintText("+--------------------------+\r\n");
}

#pragma vector = PORT1_VECTOR
__interrupt void Port_1_ISR(void) {
    if (P1IFG & PIR_PIN) {
        if (P1IN & PIR_PIN) {
            g_motion_detected = true;
            P1IES |= PIR_PIN;
        } else {
            g_motion_detected = false;
            P1IES &= ~PIR_PIN;
        }

        P1IFG &= ~PIR_PIN;
    }
}
