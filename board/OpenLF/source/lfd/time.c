/* RTC interface
 *
 * Copyright (c) 2010 LeapFrog Enterprises Inc.
 *
 * Andrey Yurovsky <ayurovsky@leapfrog.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <linux/rtc.h>

#include "debug.h"

int time_set_rtc(void)
{
	struct rtc_time rtc_tm;
	time_t systime;
	struct tm *systm;
	int fd;
	int ret;

	fd = open("/dev/rtc0", O_RDONLY);
	if (fd < 0) {
		dbprintf("can't open RTC device\n");
		return -ENODEV;
	}

	ret = ioctl(fd, RTC_RD_TIME, &rtc_tm);
	if (ret < 0) {
		dbprintf("can't retrive RTC value\n");
		close(fd);
		return -EFAULT;
	}

	dbprintf("RTC time is: %d-%d-%d, %02d:%02d:%02d.\n",
			rtc_tm.tm_mday, rtc_tm.tm_mon + 1,
			rtc_tm.tm_year + 1900, rtc_tm.tm_hour,
			rtc_tm.tm_min, rtc_tm.tm_sec);

	systime = time(NULL);
	systm = localtime(&systime);

	dbprintf("system time is: %d-%d-%d, %02d:%02d:%02d.\n",
			systm->tm_mday, systm->tm_mon + 1,
			systm->tm_year + 1900, systm->tm_hour,
			systm->tm_min, systm->tm_sec);

	ret = ioctl(fd, RTC_SET_TIME, systm);
	if (ret < 0) {
		dbprintf("can't set RTC value\n");
		close(fd);
		return -EFAULT;
	}

	close(fd);
	return 0;
}
