/* 
 * Copyright (c) 2010 LeapFrog Enterprises Inc.
 *
 * Andrey Yurovsky <ayurovsky@leapfrog.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __BACKLIGHT_H__
#define __BACKLIGHT_H__

int backlight_connect(void);
int backlight_next(void);
int backlight_get(void);
int backlight_set(int level);

#endif /* __BACKLIGHT_H__ */
