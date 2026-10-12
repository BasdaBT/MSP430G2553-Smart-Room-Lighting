// hw_control.h
#ifndef HW_CONTROL_H_
#define HW_CONTROL_H_

#include <stdbool.h>

unsigned int LDR_ReadRaw(void);
unsigned int LDR_CalculatePercentage(unsigned int rawADC);
bool PIR_IsMotionDetected(void);
void PWM_SetDuty(unsigned int duty);
unsigned int PWM_GetDuty(void);

#endif
