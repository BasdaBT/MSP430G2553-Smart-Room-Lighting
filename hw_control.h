// hw_control.h
#ifndef HW_CONTROL_H_
#define HW_CONTROL_H_

#include <stdbool.h>

#define PWM_PERIOD 100

// --- LDR Fonksiyonları ---
unsigned int LDR_ReadRaw(void);
unsigned int LDR_CalculatePercentage(unsigned int rawADC);

// --- PIR Fonksiyonları ---
bool PIR_IsMotionDetected(void);

// --- PWM Fonksiyonları ---
void PWM_SetDuty(unsigned int duty);

#endif