/* 
 * Copyright (c) 2010 LeapFrog Enterprises Inc.
 *
 * Andrey Yurovsky <ayurovsky@leapfrog.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __SOUND_H__
#define __SOUND_H__

int sound_connect(void);
void sound_disconnect(void);
int sound_volume_up(void);
int sound_volume_down(void);
int sound_get_volume(void);
void sound_set_volume(int level);

#endif /* __SOUND_H__ */
