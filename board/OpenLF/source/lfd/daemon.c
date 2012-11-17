/* daemonize
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
#include <syslog.h>
#include <fcntl.h>
#include <dirent.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <sys/un.h>

/*
 * daemonize our process by losing the controlling shell and disconnecting from
 * the file descriptors
 */
void daemonize(const char *cmd)
{
	int i, fd0, fd1, fd2;
	pid_t pid;
	struct rlimit r1;
	struct sigaction sact;

	umask(0);

	if(getrlimit(RLIMIT_NOFILE, &r1) < 0) {
		exit(1);
	}

	/*
	 * become a session leader to lose controlling TTY
	 */

	if((pid = fork()) < 0) {
		exit(1);
	}
	else if(pid != 0) /* parent */
		exit(0);
	setsid();

	/*
	 * ensure future opens won't allocate controlling TTYs
	 */

	sact.sa_handler = SIG_IGN;
	sigemptyset(&sact.sa_mask);
	sact.sa_flags = 0;
	if(sigaction(SIGHUP, &sact, NULL) == -1) {
		exit(1);
	}
	if((pid = fork()) < 0) {
		exit(1);
	}
	else if(pid != 0) /* parent */
		exit(0);

	/*
	 * change current working directory to the root so that we won't
	 * prevent file systems from being unmounted
	 */

	if(chdir("/") < 0) {
		exit(1);
	}

	/*
	 * close all open file descriptors
	 */
	if(r1.rlim_max == RLIM_INFINITY)
		r1.rlim_max = 20; /* XXX */
	for(i = 0; i < r1.rlim_max; i++)
		close(i);

	/*
	 * attach file descriptors 0, 1, 2 to /dev/null
	 */

	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);

	/*
	 * initiate log file
	 */

	if(fd0 != 0 || fd1 != 1 || fd2 != 2) {
		exit(1);
	}
}
