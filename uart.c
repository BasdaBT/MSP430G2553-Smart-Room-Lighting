#include <msp430.h>
#include "uart.h"
#include "app_config.h"

void UART_Init(void)
{
    /* USCI_A0: P1.1 RX and P1.2 TX, 9600 baud from 1 MHz SMCLK. */
    P1SEL |= UART_RX_PIN | UART_TX_PIN;
    P1SEL2 |= UART_RX_PIN | UART_TX_PIN;

    UCA0CTL1 |= UCSWRST;
    UCA0CTL1 = UCSWRST | UCSSEL_2;
    UCA0BR0 = (unsigned char)(SYSTEM_CLOCK_HZ / UART_BAUD_RATE);
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS0;
    UCA0CTL1 &= ~UCSWRST;
}

void UART_PrintChar(char c)
{
    while (!(IFG2 & UCA0TXIFG)) {
        /* Wait until the transmit buffer is ready. */
    }
    UCA0TXBUF = (unsigned char)c;
}

void UART_PrintText(const char *text)
{
    while (*text) {
        UART_PrintChar(*text++);
    }
}

bool UART_IsReceiveReady(void)
{
    return (IFG2 & UCA0RXIFG) != 0;
}

char UART_ReceiveChar(void)
{
    while (!UART_IsReceiveReady()) {
        /* Blocking receive for optional command handling. */
    }
    return (char)UCA0RXBUF;
}
