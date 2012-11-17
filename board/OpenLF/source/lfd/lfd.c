/* lfd -- the LeapFrog low-level events daemon
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
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <signal.h>
#include <unistd.h>
#include <poll.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "debug.h"
#include "socket.h"
#include "lfd.h"
#include "sound.h"
#include "backlight.h"
#include "time.h"
#include "battery.h"

#ifndef DEBUG_NO_DAEMON
#include "daemon.h"
#else
#define daemonize(a)
#endif

static int backlight = 0;
static int running = 0;
static int mv = 0;

/*
 * find and open an input device by name
 */
int open_input_device(char *input_name)
{
	struct dirent *dp;
	DIR *dir;
	char dev[20];
	char name[32];
	int fd;

	dir = opendir("/dev/input/");
	if (!dir)
		return -1;

	while ((dp = readdir(dir)) != NULL) {
		if (dp->d_name && !strncmp(dp->d_name, "event", 5)) {
			sprintf(dev, "/dev/input/%s", dp->d_name);

			fd = open(dev, O_RDONLY);
			if(fd == -1)
				continue;

			if(ioctl(fd, EVIOCGNAME(32), name) < 0) {
				close(fd);
				continue;
			}

			if(!strcmp(name, input_name)) { /* found */
				closedir(dir);
				return fd;
			}

			close(fd);
		}
	}

	closedir(dir);
	return -1;
}

static void handle_control_message(struct lfd_control_message *msg, int s)
{
	int ret = 0;

	switch (msg->command) {
		case LFD_COMMAND_GET_VOLUME:
			msg->payload = sound_get_volume();
			msg->command |= LFD_RESPONSE;
			break;

		case LFD_COMMAND_SET_VOLUME:
			sound_set_volume(msg->payload);
			break;

		case LFD_COMMAND_GET_BACKLIGHT:
			msg->payload = backlight_get();
			msg->command |= LFD_RESPONSE;
			break;

		case LFD_COMMAND_SET_BACKLIGHT:
			backlight_set(msg->payload);
			break;

		case LFD_COMMAND_SET_BACKLIGHT_NEXT:
			backlight_next();
			break;

		case LFD_COMMAND_SET_RTC_TO_SYSTIME:
			time_set_rtc();
			break;

		case LFD_COMMAND_GET_BATTERY_MV:
			msg->payload = mv;
			msg->command |= LFD_RESPONSE;
			break;

		default:
			dbprintf("unknown LFD command: 0x%X\n", msg->command);
			return;
	};

	if (msg->command & LFD_RESPONSE) {
		ret = send(s, msg, sizeof(*msg), MSG_NOSIGNAL);
		if (ret < 0)
			dbprintf("can't send response\n");
	}
}

void handle_control_socket(int s)
{
	ssize_t nb;
	ssize_t len = 0;
	struct lfd_control_message msg;
	struct sockaddr_un app;
	socklen_t s_app = sizeof(struct sockaddr_un);

	int ms = accept(s, (struct sockaddr *)&app, &s_app);

	if (ms <= 0)
		return;

	while (len < sizeof(msg)) {
		nb = recv(ms, &msg, sizeof(msg), 0);
		if (nb < 0) {
			close(ms);
			return;
		}
		len += nb;
	}
	
	handle_control_message(&msg, ms);
	close(ms);
}

void handle_key(unsigned int code)
{
	switch (code) {
	       case KEY_VOLUMEUP:
		       sound_volume_up();
		       break;
	       case KEY_VOLUMEDOWN:
		       sound_volume_down();
		       break;
	       case KEY_X:
		       if (backlight)
			       backlight_next();
		       break;
	}
}

/*
 * handle signals
 */
void handle_signal(int sig)
{
	switch(sig) {
		/* the daemon needs to shut down */
		case SIGTERM:
		case SIGINT:
			running = 0;
		break;

		default:
		dbprintf("unknown signal\n");
		break;
	}
}

enum {
	LFD_KEY_PRESS		= 0,
	LFD_CONTROL_SOCKET	= 1,
};

int main(int argc, char **argv)
{
	int ret, size;
	int fd = -1, ls;
	struct input_event ev;
	struct pollfd fds[2];
	int num_fds = 0;
	struct sigaction sa_int;

	/* 
	 * initialize our state
	 */

	daemonize(argv[0]);	

#ifdef DEBUG_PRINT
	console = fopen("/dev/console", "w");
	if(console == NULL)
		exit(1);
#endif

	if (sound_connect())
		goto fail_sound;

	running = 0;
	if (backlight_connect() == 0)
		backlight = 1;

	fd = open_input_device("LF1000 Keyboard");
	if (fd < 0) {
		dbprintf("can't open input device\n");
		goto fail_kb;
	}
	fds[LFD_KEY_PRESS].fd = fd;
	fds[LFD_KEY_PRESS].events = POLLIN;
	num_fds++;

	ls = create_listening_socket(LFD_SOCKET_PATH);
	if (ls < 0) {
		dbprintf("can't create listening socket\n");
		goto fail_sock;
	}
	fcntl(ls, F_SETFL, O_NONBLOCK);
	fds[LFD_CONTROL_SOCKET].fd = ls;
	fds[LFD_CONTROL_SOCKET].events = POLLIN;
	num_fds++;

	mv = battery_get_mv();

	/*
	 * trap SIGTERM so that we clean up before exiting
	 */

	running = 1;
        sigemptyset(&sa_int.sa_mask);
	sa_int.sa_handler = handle_signal;
	sa_int.sa_flags = 0;
	if (sigaction(SIGTERM, &sa_int, NULL) == -1) {
		dbprintf("can't trap SIGTERM\n");
		goto fail_sig;
	}

	if (sigaction(SIGINT, &sa_int, NULL) == -1) {
		dbprintf("can't trap SIGINT\n");
		goto fail_sig;
	}

	/*
	 * monitor
	 */

	while (running) {
		ret = poll(fds, num_fds, 1000);
		if (ret == 0) { /* timeout */
			mv = battery_get_mv();
		} else if (ret > 0) {
			if (fds[LFD_KEY_PRESS].revents & POLLIN) {
				size = read(fd, &ev, sizeof(ev));
				if (ev.type == EV_KEY && ev.value == 1)
					handle_key(ev.code);
			}
			if (fds[LFD_CONTROL_SOCKET].revents & POLLIN)
				handle_control_socket(ls);
		}
	}

	dbprintf("exiting...\n");

	close(ls);
	close(fd);
	sound_disconnect();
#ifdef DEBUG_PRINT
	fclose(console);
#endif
	exit(0);

fail_sig:
	close(ls);
fail_sock:
fail_sound:
	close(fd);
fail_kb:
	sound_disconnect();
#ifdef DEBUG_PRINT
	fclose(console);
#endif
	exit(1);
}
