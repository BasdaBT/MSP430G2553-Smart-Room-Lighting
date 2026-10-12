#ifndef APP_CONFIG_H_
#define APP_CONFIG_H_

#include <msp430.h>

/* MSP430G2553 pin assignments from the project report. */
#define LDR_PIN                         BIT0   /* P1.0 / A0 */
#define PIR_PIN                         BIT5   /* P1.5 */
#define PWM_LED_PIN                     BIT1   /* P2.1 / TA1.1 */
#define UART_RX_PIN                     BIT1   /* P1.1 / UCA0RXD */
#define UART_TX_PIN                     BIT2   /* P1.2 / UCA0TXD */
#define I2C_SCL_PIN                     BIT6   /* P1.6 / UCB0SCL */
#define I2C_SDA_PIN                     BIT7   /* P1.7 / UCB0SDA */

#define SYSTEM_CLOCK_HZ                 1000000UL
#define UART_BAUD_RATE                  9600UL
#define I2C_CLOCK_DIVIDER               10U

/* Calibrate these two values for the installed LDR divider. */
#define LDR_DARK_RAW                    1U
#define LDR_BRIGHT_RAW                  1004U
#define LIGHT_THRESHOLD_PERCENT         40U

#define PWM_PERIOD_TICKS                100U
#define BOOT_STEP_DELAY_CYCLES          250000UL
#define MAIN_LOOP_DELAY_CYCLES          100000UL
#define TELEMETRY_PERIOD_LOOPS          10U
#define I2C_TIMEOUT_COUNT               60000U

#endif
