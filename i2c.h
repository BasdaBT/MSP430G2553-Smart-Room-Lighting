/*
 * i2c.h
 *
 *  Created on: May 19, 2019
 *      Author: Sam
 */

#ifndef I2C_H_
#define I2C_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <msp430.h>
#include <stdbool.h>
#include <stdint.h>

void i2c_init(void);
bool i2c_write(uint8_t slave_address, const uint8_t *data, uint8_t length);

#ifdef __cplusplus
}
#endif

#endif /* I2C_H_ */
