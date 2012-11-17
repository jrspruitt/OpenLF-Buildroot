/* high-level battery monitoring routines
 *
 * Copyright (c) 2010 LeapFrog Enterprises Inc.
 *
 * Andrey Yurovsky <ayurovsky@leapfrog.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "debug.h"

#define ADC_VBAT_SYSFS	"/sys/devices/platform/lf1000-adc/channel2"

/* model ADC as a line:
 * milliVolts = ((ADC_SLOPE * 256) * READING) / 256 + ADC_CONSTANT */  
#define ADC_SLOPE_256	2012	/* ADC slope * 256 */
#define ADC_CONSTANT	0	/* ADC constant */


int battery_get_mv(void)
{
	FILE *f;
	unsigned int reading;

	f = fopen(ADC_VBAT_SYSFS, "r");
	if (!f) {
		dbprintf("can't open " ADC_VBAT_SYSFS "\n");
		return -ENODEV;
	}

	if (fscanf(f, "%u", &reading) != 1) {
		dbprintf("can't read ADC value\n");
		fclose(f);
		return -EINVAL;
	}

	fclose(f);
	
	return ((ADC_SLOPE_256*reading)/256)+ADC_CONSTANT;
}
