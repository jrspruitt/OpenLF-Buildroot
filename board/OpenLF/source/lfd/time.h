/* 
 * Copyright (c) 2010 LeapFrog Enterprises Inc.
 *
 * Andrey Yurovsky <ayurovsky@leapfrog.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __LFD_TIME_H__
#define __LFD_TIME_H__

/* Set the RTC time to the current system time.  Set the system time as desired
 * before calling this function. */
int time_set_rtc(void);

#endif /* __LFD_TIME_H__ */
