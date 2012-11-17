/* high-level backlight control for LeapFrog boards
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
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "debug.h"
#include "nor.h"
#include "i2c.h"

/* We support three backlight interfaces:
 * 1. standard Linux framebuffer backlight API.  The backlight driver in turn
 *    talks to the backlight hardware.
 * 2. LF graphics driver API with sysfs backlight control.  The driver controls
 *    the backlight via PWM.
 * 3. LF graphics driver API without backlight control, in which case we talk
 *    to the LFP100 chip to control the backlight.
 */

enum bl_type {
	BL_NONE			= 0,
	BL_STANDARD		= 1,
	BL_NON_STANDARD_PWM	= 2,
	BL_NON_STANDARD_I2C	= 3,
};

struct bl_priv {
	enum bl_type	type;

	union {
		struct {
			char driver_name[32];
			unsigned int max;
			int next;
			int current;
		} std;
	} info;

	int blcal;
};

static struct bl_priv priv = {
	.type	= BL_NONE,
};

/* non-standard PWM control */
#define BL_NONSTD_DRIVER	"/sys/devices/platform/lf1000-dpc/"
#define BL_LOGICAL		"backlight_logical"

/* standard */
#define BL_DRIVER		"/sys/class/backlight"
#define BL_RANGE_LOW		0
#define BL_RANGE_HIGH		511

#define LFP100_ADDR	0xCC

/*
 * match existing Explorer backlight levels
 */

#define	PHYSICAL_MIN_BACKLIGHT	( 164 )

static int backlight[4] = { PHYSICAL_MIN_BACKLIGHT, 234, 318, 400 };

#define BL_INIT_LEVEL	318
#define BL_INIT_NEXT	1

/* max backlight values */
#define BL_RANGE_LOW	0
#define BL_RANGE_HIGH	511
#define BL_LOGICAL_LOW	0
#define BL_LOGICAL_HIGH	( sizeof(backlight) / sizeof(int) - 1 )

/*
 * convert virtual byte range [-128,127] to physical [0,511] value
 */
static int virtual_to_physical(int value)
{
	if (value < -128)
		value = -128;
	else if (value > 127)
		value = 127;

	return(value * 2 + 256);
}

static int value_to_level(int value)
{
	int i;

	for (i=0; i <= BL_LOGICAL_HIGH; i++) {
		if (value <= backlight[i])
			return (i);
	}

	/* value not found, return max level */
	return(BL_LOGICAL_HIGH);
}

static int standard_backlight_init(void)
{
	struct stat st;
	struct dirent *dp;
	DIR *dir;
	char buf[64];
	FILE *f;

	dbprintf("%s", __FUNCTION__);

	if (stat(BL_DRIVER, &st)) {
		dbprintf("no " BL_DRIVER "\n");
		return 0;
	}

	dir = opendir(BL_DRIVER);
	if (!dir) {
		dbprintf("can't open " BL_DRIVER "\n");
		return 0;
	}

	while ((dp = readdir(dir)) != NULL) {
		if (dp->d_name && dp->d_name[0] != '.') {
			snprintf(priv.info.std.driver_name, 31, "%s",
					dp->d_name);
		
			dbprintf("checking \"%s\"\n",
					priv.info.std.driver_name);

			/* save the max brightness */

			snprintf(buf, 63, BL_DRIVER "/%s/max_brightness",
					priv.info.std.driver_name);
			f = fopen(buf, "r");
			if (!f) {
				dbprintf("can't open %s\n", buf);
				continue;
			}
			if (fscanf(f, "%u", &priv.info.std.max) != 1) {
				dbprintf("can't read max_brightness\n");
				continue;
			}

			priv.info.std.next = BL_INIT_NEXT;

			/* all good */

			closedir(dir);
			return 1;
		}
	}

	closedir(dir);
	return 0;
}

static int lfp100_detected(void)
{
	uint8_t val;
	
	if (i2c_read(0, LFP100_ADDR, 0x00, &val, 1) < 0)
		return 0;

	if ((val & 0xF0) != 0x00)
		return 0;

	dbprintf("found LFP100r%d\n", val & 0xF);
	return 1;
}

static FILE *bl_fopen_standard_brightness(void)
{
	char buf[64];

	snprintf(buf, 63, BL_DRIVER "/%s/brightness",
			priv.info.std.driver_name);
	
	return fopen(buf, "r+");
}

static void bl_set_standard(int level)
{	
	unsigned int val;
	FILE *f;

	dbprintf("%s\n", __FUNCTION__);

	f = bl_fopen_standard_brightness();
	if (!f) {
		dbprintf("can't open brightness sysfs hook\n");
		return;
	}

	if (level < BL_LOGICAL_LOW)
		level = BL_LOGICAL_LOW;
	else if (level > BL_LOGICAL_HIGH)
		level = BL_LOGICAL_HIGH;

	val = backlight[level] + priv.blcal;

	/* ensure val is in valid range */
	if      (val < BL_RANGE_LOW) val  = BL_RANGE_LOW;
	else if (val > BL_RANGE_HIGH) val = BL_RANGE_HIGH;

	dbprintf("setting level %u (val=%u)\n", level, val);

	fprintf(f, "%u", val);
	fclose(f);
}

static int bl_get_standard(void)
{
	FILE *f;
	unsigned int val;

	dbprintf("%s.%d\n", __FUNCTION__, __LINE__);

	f = bl_fopen_standard_brightness();
	if (!f) {
		dbprintf("can't open brightness sysfs hook\n");
		return 0;
	}

	if (fscanf(f, "%u", &val) != 1) {
		dbprintf("can't read brightness value\n");
		fclose(f);
		return 0;
	}

	fclose(f);

	dbprintf("%s.%d val=%u, level=%d\n", __FUNCTION__, __LINE__,
		val, value_to_level(val - priv.blcal));
	return(value_to_level(val - priv.blcal));	/* convert to level */
}

static void bl_next_standard(void)
{
	FILE *f;
	unsigned int val;
	int next_logical;

	f = bl_fopen_standard_brightness();
	if (!f) {
		dbprintf("can't open brightness sysfs hook\n");
		return;
	}

	next_logical = priv.info.std.current + priv.info.std.next;

	dbprintf("%s.%d priv.info.std.current=%d, next_logical=%d, next=%d\n",
		__FUNCTION__, __LINE__,
		priv.info.std.current, next_logical, priv.info.std.next);
	if (next_logical < 0) {
		next_logical = 1;
		priv.info.std.next = 1;
	} else if (next_logical > BL_LOGICAL_HIGH) {
		next_logical = BL_LOGICAL_HIGH - 1;
		priv.info.std.next = -1;
	}

	fprintf(f, "%u", backlight[next_logical] + priv.blcal);
	priv.info.std.current = next_logical;
	fclose(f);
}

static void bl_set_nonstandard_pwm(int level)
{
	FILE *f;

	f = fopen(BL_NONSTD_DRIVER "backlight_logical", "w");
	if (!f)
		return;

	fprintf(f, "%d", level);
	fclose(f);
}

static int bl_get_nonstandard_pwm(void)
{
	unsigned int val;
	FILE *f;

	f = fopen(BL_NONSTD_DRIVER "backlight_logical", "r");
	if (!f)
		return 0;

	fscanf(f, "%u", &val);

	fclose(f);
	return val;
}

static void bl_next_nonstandard_pwm(void)
{
	FILE *f;

	f = fopen(BL_NONSTD_DRIVER "backlight_next", "w");
	if (!f)
		return;

	fprintf(f, "%d", 1);
	
	fclose(f);
}

static void bl_set_nonstandard_i2c(int level)
{
	FILE *f;

	f = fopen(BL_NONSTD_DRIVER "backlight_logical", "w");
	if (!f)
		return;

	fprintf(f, "%d", level);
	fclose(f);
}

static int bl_get_nonstandard_i2c(void)
{
	unsigned int val;
	FILE *f;

	f = fopen(BL_NONSTD_DRIVER "backlight_logical", "r");
	if (!f)
		return 0;

	fscanf(f, "%u", &val);

	fclose(f);
	return val;
}

static void bl_next_nonstandard_i2c(void)
{
	FILE *f;

	f = fopen(BL_NONSTD_DRIVER "backlight_next", "w");
	if (!f)
		return;

	fprintf(f, "%d", 1);
	
	fclose(f);
}

int backlight_connect(void)
{
	struct stat st;
	int virtual_calibration;

	dbprintf("%s\n", __FUNCTION__);

	priv.type = BL_NONE;
	priv.blcal = 0;

	if (standard_backlight_init()) {
		dbprintf("found backlight driver \"%s\", max brightness %u\n",
				priv.info.std.driver_name,
				priv.info.std.max);
		priv.type = BL_STANDARD;
	} else if (!stat(BL_NONSTD_DRIVER, &st)) {
		if (lfp100_detected()) {
			dbprintf("found non-standard LFP100 backlight\n");
			priv.type = BL_NON_STANDARD_I2C;
		} else if (!stat(BL_NONSTD_DRIVER BL_LOGICAL, &st)) {
			dbprintf("found non-standard PWM backlight\n");
			priv.type = BL_NON_STANDARD_PWM;
		}
	}

	if (priv.type == BL_NONE) {
		dbprintf("error: no backlight found\n");
		return -ENODEV;
	}

	/* read virtual minimum backlight */
	if (!nor_open()) {
		if (nor_get_field(NOR_FIELD_BACKLIGHT, &virtual_calibration,
					sizeof(virtual_calibration))) {
			dbprintf("error: can't read backlight calibration\n");
		} else {
			//Minh Saelock, reverting this back to a strict offset, and ignoring values less then 0.
			//This is to match behavior of old dpc driver.
			//priv.blcal = virtual_to_physical(virtual_calibration) -
			//	PHYSICAL_MIN_BACKLIGHT;
			if(virtual_calibration > 0) {
				priv.blcal = virtual_calibration;
				dbprintf("backlight physical offset %d\n",
						priv.blcal);
			}
		}
	} else {
		dbprintf("error: no NOR Flash access for BL calibration\n");
	}
	//Initialize brightness to 2 bars.
	backlight_set(2);
	priv.info.std.current = 2;

	return 0;
}

int backlight_next(void)
{
	dbprintf("%s\n", __FUNCTION__);

	switch (priv.type) {
		case BL_NON_STANDARD_PWM:
			bl_next_nonstandard_pwm();
			break;
		case BL_NON_STANDARD_I2C:
			bl_next_nonstandard_i2c();
			break;
		case BL_STANDARD:
			bl_next_standard();
			break;
		default:
			return -EINVAL;
	}
	return 0;
}

int backlight_get(void)
{
	dbprintf("%s\n", __FUNCTION__);

	switch (priv.type) {
		case BL_NON_STANDARD_PWM:
			return bl_get_nonstandard_pwm();
		case BL_NON_STANDARD_I2C:
			return bl_get_nonstandard_i2c();
		case BL_STANDARD:
			return bl_get_standard();
		default:
			return -EINVAL;
	}
	return 0;
}

int backlight_set(int level)
{
	dbprintf("%s\n", __FUNCTION__);

	switch (priv.type) {
		case BL_NON_STANDARD_PWM:
			bl_set_nonstandard_pwm(level);
			break;
		case BL_NON_STANDARD_I2C:
			bl_set_nonstandard_i2c(level);
			break;
		case BL_STANDARD:
			bl_set_standard(level);
			break;
		default:
			return -EINVAL;
	}
	return 0;
}
