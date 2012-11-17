/* control protocol for the LeapFrog low-level events daemon
 *
 * Copyright (c) 2010 LeapFrog Enterprises Inc.
 *
 * Andrey Yurovsky <ayurovsky@leapfrog.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __LFD_H__
#define __LFD_H__

#include <stdint.h>

#define LFD_SOCKET_PATH	"/var/run/lfd-control"

#define LFD_RESPONSE	(1<<31)

enum lfd_command {
	LFD_COMMAND_GET_VOLUME		= 0,
	LFD_COMMAND_SET_VOLUME		= 1,
	LFD_COMMAND_GET_BACKLIGHT	= 2,
	LFD_COMMAND_SET_BACKLIGHT	= 3,
	LFD_COMMAND_SET_BACKLIGHT_NEXT	= 4,
	LFD_COMMAND_SET_RTC_TO_SYSTIME	= 5,
	LFD_COMMAND_GET_BATTERY_MV	= 6,

	LFD_LAST_COMMAND
};

struct lfd_control_message {
	uint32_t	command;
	uint32_t	payload;
};

#endif /* __LFD_H__ */
