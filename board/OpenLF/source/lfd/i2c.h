/* I2C transfers from user space via the i2c chardev driver
 *
 * Copyright (c) 2010 LeapFrog Enterprises Inc.
 *
 * Andrey Yurovsky <ayurovsky@leapfrog.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __USER_I2C_H__
#define __USER_I2C_H__

#include <stdint.h>

int i2c_read(unsigned chan, uint8_t addr, uint8_t reg, uint8_t *buf,
	unsigned count);
int i2c_write(unsigned chan, uint8_t addr, uint8_t reg, uint8_t *buf,
		unsigned count);

#endif /* __USER_I2C_H__ */
