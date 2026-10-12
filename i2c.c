#include "i2c.h"
#include "app_config.h"

void i2c_init(void)
{
    P1SEL |= I2C_SCL_PIN | I2C_SDA_PIN;
    P1SEL2 |= I2C_SCL_PIN | I2C_SDA_PIN;

    UCB0CTL1 |= UCSWRST;
    UCB0CTL0 = UCMST | UCMODE_3 | UCSYNC;
    UCB0CTL1 = UCSSEL_2 | UCSWRST;
    UCB0BR0 = I2C_CLOCK_DIVIDER;
    UCB0BR1   = 0;
    UCB0CTL1 &= ~UCSWRST;
}

bool i2c_write(uint8_t slave_address, const uint8_t *data, uint8_t length)
{
    uint16_t timeout = I2C_TIMEOUT_COUNT;
    uint8_t index;

    if ((data == 0) || (length == 0)) {
        return false;
    }

    while ((UCB0STAT & UCBBUSY) && (--timeout != 0)) {
        /* Wait for a previous transaction to finish. */
    }
    if (timeout == 0) {
        return false;
    }

    UCB0I2CSA = slave_address;
    UCB0STAT &= ~UCNACKIFG;
    UCB0CTL1 |= UCTR | UCTXSTT;

    for (index = 0; index < length; ++index) {
        timeout = I2C_TIMEOUT_COUNT;
        while (!(IFG2 & UCB0TXIFG)) {
            if ((UCB0STAT & UCNACKIFG) || (--timeout == 0)) {
                goto i2c_error;
            }
        }
        UCB0TXBUF = data[index];
    }

    timeout = I2C_TIMEOUT_COUNT;
    while (!(IFG2 & UCB0TXIFG)) {
        if ((UCB0STAT & UCNACKIFG) || (--timeout == 0)) {
            goto i2c_error;
        }
    }

    UCB0CTL1 |= UCTXSTP;
    timeout = I2C_TIMEOUT_COUNT;
    while ((UCB0CTL1 & UCTXSTP) && (--timeout != 0)) {
        /* Wait until the stop condition leaves the bus. */
    }
    return timeout != 0;

i2c_error:
    UCB0CTL1 |= UCTXSTP;
    UCB0STAT &= ~UCNACKIFG;
    return false;
}
