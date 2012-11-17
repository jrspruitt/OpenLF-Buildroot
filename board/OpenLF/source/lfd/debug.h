/* 
 * Copyright (c) 2010 LeapFrog Enterprises Inc.
 *
 * Andrey Yurovsky <ayurovsky@leapfrog.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __DEBUG_H__
#define __DEBUG_H__

#ifdef DEBUG_PRINT
#include <stdio.h>
#define dbprintf(...)   fprintf(console, "lfd: " __VA_ARGS__)
FILE *console;  /* for debug printing */
#else
#define dbprintf(...)
#endif

#endif /* __DEBUG_H__ */
