# MSP430G2553 Smart Room Lighting and Energy Saving System

An automatic lighting controller built around the MSP430G2553. The firmware
combines ambient-light measurement and motion detection so that the LED is
enabled only when somebody is present and the room is dark. LED brightness is
adjusted with PWM, while an SSD1306 OLED and an HC-05 Bluetooth module expose
the current system state.

## Features

- ADC10 measurement of an LDR voltage divider
- Interrupt-driven HC-SR501 PIR motion detection
- Timer1_A PWM output through a 2N2222 LED driver
- SSD1306 128x64 OLED over USCI_B0 I2C
- HC-05 telemetry over USCI_A0 hardware UART at 9600 baud
- Filtered light percentage to reduce display and PWM jitter
- Bounded I2C transactions so a disconnected display cannot lock the firmware

## Control logic

| Room state | Motion | LED |
| --- | --- | --- |
| Bright | Any | Off |
| Dark | No | Off |
| Dark | Yes | On; duty cycle increases as the room gets darker |

The default darkness threshold is 40%. Calibration constants and timing values
are centralized in `app_config.h`.

## Pin map

| Function | MSP430G2553 pin | Peripheral |
| --- | --- | --- |
| LDR divider | P1.0 | ADC10 A0 |
| PIR output | P1.5 | GPIO interrupt |
| LED driver PWM | P2.1 | Timer1_A.1 |
| HC-05 RX (from MSP430 TX) | P1.2 | UCA0TXD |
| HC-05 TX (to MSP430 RX) | P1.1 | UCA0RXD |
| OLED SCL | P1.6 | UCB0SCL |
| OLED SDA | P1.7 | UCB0SDA |

Use a common ground for every module. The LED must be driven through the
2N2222 stage described in the report; do not power a high-current LED directly
from an MSP430 pin.

## Project structure

- `main.c` - application state machine, OLED UI, telemetry and PIR ISR
- `app_config.h` - pins, thresholds, calibration and timing
- `init.c/.h` - clock, ADC, PIR and PWM initialization
- `hw_control.c/.h` - LDR filtering, PIR readout and PWM control
- `uart.c/.h` - USCI_A0 hardware UART driver
- `i2c.c/.h` - timeout-protected USCI_B0 I2C writer
- `ssd1306.c/.h`, `font_5x7.h` - OLED text driver
- `targetConfigs/` - Code Composer Studio target configuration
- [`docs/Project_Report.pdf`](docs/Project_Report.pdf) - complete project report
- `docs/TEST_PLAN.md` - hardware verification checklist
- `Image's/` - reference PCB and schematic PDFs from the upstream project

## Build and flash

1. Install Texas Instruments Code Composer Studio with MSP430 support.
2. Import this directory as an existing CCS project.
3. Select the MSP430G2553 target configuration.
4. Build the `Debug` configuration and flash the LaunchPad.
5. Open an HC-05 serial terminal at 9600 baud, 8 data bits, no parity and one
   stop bit.

Generated `Debug` and `Release` outputs are intentionally excluded from Git.

## LDR calibration

1. Observe the raw ADC value with the LDR covered and set `LDR_DARK_RAW`.
2. Observe the value under the brightest expected lighting and set
   `LDR_BRIGHT_RAW`.
3. Adjust `LIGHT_THRESHOLD_PERCENT` for the room.

## Validation

The code has been reviewed against the project report and the documented pin
map. It still requires a final build in CCS and hardware verification with the
actual LDR, PIR, OLED, HC-05 and transistor stage.

## Attribution

The initial CCS project and OLED support code were adapted from
[OmerSahinkou/Microprocessor_2025-2026](https://github.com/OmerSahinkou/Microprocessor_2025-2026).
See `THIRD_PARTY_NOTICES.md` for details.
