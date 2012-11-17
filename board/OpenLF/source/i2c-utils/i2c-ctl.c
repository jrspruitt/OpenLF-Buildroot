/* i2c-ctl: do I2C transfers from user space via the i2c chardev driver
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
#include <sys/ioctl.h>
#include <errno.h>

#include "i2c.h"
#include "i2c-dev.h"

static void print_usage(void) {
	printf( "usage: i2c-ctl <device> w <address> <byte1> <byte2> ...\n"
		"       i2c-ctl <device> r <address> <byte1> <num_bytes>\n"
		"\n"
		"Example: 'i2c-ctl /dev/i2c-0 r 0x95 0x01 2' uses i2c\n"
		"         channel 0 (/dev/i2c-0) addressing the chip at \n"
		"         0x95 register 1 (0x01) and reads 2 bytes\n");
}

int main(int argc, char **argv)
{
	int fd;
	int ret, i, num;
	int funcs;
	unsigned char buf[64];

	struct i2c_msg messages[2];
	struct i2c_rdwr_ioctl_data msgset;

	msgset.msgs = messages;

	messages[0].flags = 0;
	messages[1].flags = I2C_M_RD;

	if (argc < 5 || ((argv[2][0] == 'r') && argc < 6)) {
		print_usage();
		return 0;
	}

	if (argv[2][0] != 'w' && argv[2][0] != 'r') {
		printf("invalid argument\n");
		print_usage();
		return 1;
	}

	fd = open(argv[1], O_RDWR);
	if (fd < 0) {
		printf("error: failed to open '%s' (%m).\n", argv[1]);
		close(fd);
		print_usage();
		return 1;
	}
	
	/* sanity-check */

	ret = ioctl(fd, I2C_FUNCS, &funcs);
	if (ret < 0) {
		printf("error: failed to get i2c functionality.\n");
		close(fd);
		print_usage();
		return 1;
	}

	if (!(funcs & I2C_FUNC_I2C)) {
		perror("error: adapter doesn't support normal i2c.\n");
		close(fd);
		print_usage();
		return 1;
	}

	if (argv[2][0] == 'w') { /* WRITE */
		for(i = 3; i < argc; i++) /* convert */
			buf[i-3] = (unsigned char)strtol(argv[i], NULL, 0);

		msgset.nmsgs = 1;
		messages[0].addr = buf[0];
		messages[0].buf = &buf[1];
		messages[0].len = argc-4;
		ret = ioctl(fd, I2C_RDWR, &msgset);
		if (ret < 0) {
			perror("failed to write.\n");
			close(fd);
			return 1;
		}
	} else if (argv[2][0] == 'r') { /* READ */
		msgset.nmsgs = 2;
		/* first, send the address+byte1 as a write */
		num = strtol(argv[3],NULL,0);	// address
		messages[0].addr = (unsigned char)num;
		messages[1].addr = (unsigned char)num;
		num = strtol(argv[4],NULL,0);	// byte 1
		buf[0] = (unsigned char)num;
		messages[0].buf = &buf[0];
		messages[0].len = 1;

		num = strtol(argv[5],NULL,0);	// number of bytes to read
		messages[1].buf = &buf[1];
		messages[1].len = num;

		/* clear out the buffer */
		for(i = 1; i < num; i++)
			buf[i] = 0;

		ret = ioctl(fd, I2C_RDWR, &msgset);
		if(ret < 0) {
			perror("failed to write/read.\n");
			close(fd);
			return 1;
		}
		for(i = 0; i < num; i++)
			printf("0x%02X ", messages[1].buf[i]);
		putchar('\n');
	} else {
		print_usage();
	}

	/* done */
	close(fd);
	return 0;
}
