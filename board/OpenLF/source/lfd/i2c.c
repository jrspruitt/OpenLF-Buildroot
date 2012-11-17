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
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

static int i2c_open(unsigned chan)
{
	int fd;
	char name[16];
	int funcs;
       
	if (snprintf(name, 16, "/dev/i2c-%u", chan) < 0)
		return -EINVAL;

	fd = open(name, O_RDWR);
	if (fd < 0)
		return fd;

	/* sanity-check */

	if (ioctl(fd, I2C_FUNCS, &funcs) < 0 || !(funcs & I2C_FUNC_I2C)) {
		close(fd);
		return -EINVAL;
	}

	return fd;
}

int i2c_read(unsigned chan, uint8_t addr, uint8_t reg, uint8_t *buf,
	unsigned count)
{
	int fd;
	int ret;
	struct i2c_msg messages[2];
	struct i2c_rdwr_ioctl_data msgset;

	fd = i2c_open(chan);
	if (fd < 0)
		return fd;

	/* first, send the address+reg as a write */
	messages[0].addr = addr;
	messages[0].buf = &reg;
	messages[0].len = 1;
	messages[0].flags = 0; /* write */

	/* then read into the buffer */
	messages[1].addr = addr;
	messages[1].buf = buf;
	messages[1].len = count;
	messages[1].flags = I2C_M_RD;

	memset(buf, 0, count);
	msgset.msgs = messages;
	msgset.nmsgs = 2;

	/* read data */
	ret = ioctl(fd, I2C_RDWR, &msgset);

	close(fd);
	return ret;
}

int i2c_write(unsigned chan, uint8_t addr, uint8_t reg, uint8_t *buf,
		unsigned count)
{
	int fd;
	int ret;
	struct i2c_msg message;
	struct i2c_rdwr_ioctl_data msgset;

	fd = i2c_open(chan);
	if (fd < 0)
		return fd;

	message.addr = addr;
	message.buf = buf;
	message.len = count;

	msgset.msgs = &message;
	msgset.nmsgs = 1;

	/* write data */
	ret = ioctl(fd, I2C_RDWR, &msgset);

	close(fd);
	return ret;
}
