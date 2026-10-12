# Hardware test plan

## Preconditions

- Verify 3.3 V and common ground before connecting the MSP430.
- Cross-connect UART: MSP430 P1.2 TX to HC-05 RX and HC-05 TX to P1.1 RX.
- Confirm the OLED address is `0x3C` and install pull-ups on SDA/SCL.
- Drive the LED through the documented 2N2222 transistor stage.

## Functional tests

| Test | Light | Motion | Expected result |
| --- | --- | --- | --- |
| 1 | Bright | No | LED off; OLED reports bright/no motion |
| 2 | Bright | Yes | LED off; Bluetooth reports daylight |
| 3 | Dark | No | LED off; OLED reports dark/waiting |
| 4 | Dark | Yes | LED on; PWM increases as light decreases |
| 5 | OLED disconnected | Any | Control loop continues without hanging |
| 6 | HC-05 connected | Any | Telemetry arrives once per second at 9600 baud |

## Calibration checks

- Record raw ADC values in the darkest and brightest operating conditions.
- Update `LDR_DARK_RAW` and `LDR_BRIGHT_RAW` in `app_config.h`.
- Confirm that the transition near `LIGHT_THRESHOLD_PERCENT` is stable.
