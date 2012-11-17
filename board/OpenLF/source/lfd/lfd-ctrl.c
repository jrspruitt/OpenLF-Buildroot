#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <poll.h>
#include <errno.h>
#include <assert.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "lfd.h"
#include "socket.h"

static int set(uint32_t command, uint32_t val)
{
	struct lfd_control_message msg;
	int sock;
	ssize_t ret;

	msg.command = command;
	msg.payload = val;
	
	sock = create_report_socket(LFD_SOCKET_PATH);
	if (sock < 0) {
		perror("socket");
		return -EPIPE;
	}

	ret = send(sock, &msg, sizeof(msg), 0);
	if (ret < 0)
		perror("send");
	else
		ret = 0;

	close(sock);
	return 0;
}

static int get(uint32_t command, uint32_t *val)
{
	struct lfd_control_message msg;
	int sock;
	ssize_t ret;

	msg.command = command;
	
	sock = create_report_socket(LFD_SOCKET_PATH);
	if (sock < 0) {
		perror("socket");
		return -EPIPE;
	}

	ret = send(sock, &msg, sizeof(msg), 0);
	if (ret < 0) {
		perror("send");
		close(sock);
		return ret;
	}

	memset(&msg, 0, sizeof(msg));
	ret = recv(sock, &msg, sizeof(msg), 0);
	if (ret < 0) {
		perror("receive");
		close(sock);
		return ret;
	}

	if (ret != sizeof(msg) || !(msg.command & LFD_RESPONSE) ||
			((msg.command & ~LFD_RESPONSE) != command))
		return -EINVAL;

	*val = msg.payload;
	close(sock);
	return 0;
}

static void print_usage(char *name)
{
	printf("usage: %s <get|set> <item> [val]\n", name);
	printf("\t\t  get volume\n");
	printf("\t\t  set volume [0-8]\n");
	printf("\t\t  get backlight\n");
	printf("\t\t  set backlight [0-3]\n");
	printf("\t\t  set backlight-next\n");
	printf("\t\t  set time\n");
	printf("\t\t  get battery\n");
}

int main(int argc, char **argv)
{
	int ret;
	uint32_t val = 0;

	if (argc < 3) {
		print_usage(argv[0]);
		return 0;
	}

	if (!strcmp(argv[1], "get")) {
		if (!strcmp(argv[2], "volume"))
			ret = get(LFD_COMMAND_GET_VOLUME, &val);
		else if (!strcmp(argv[2], "backlight"))
			ret = get(LFD_COMMAND_GET_BACKLIGHT, &val);
		else if (!strcmp(argv[2], "battery"))
			ret = get(LFD_COMMAND_GET_BATTERY_MV, &val);
		else {
			printf("invalid option\n");
			print_usage(argv[0]);
			return 1;
		}

		if (ret)
			printf("error: %d\n", ret);
		else
			printf("%d\n", val);
	} else if (!strcmp(argv[1], "set")) {
		if (argc >= 4 && sscanf(argv[3], "%u", &val) != 1) {
			printf("invalid argument\n");
			print_usage(argv[0]);
			return 1;
		}

		if (!strcmp(argv[2], "volume") && argc >= 4)
			ret = set(LFD_COMMAND_SET_VOLUME, val);
		else if (!strcmp(argv[2], "backlight") && argc >= 4)
			ret = set(LFD_COMMAND_SET_BACKLIGHT, val);
		else if (!strcmp(argv[2], "backlight-next"))
			ret = set(LFD_COMMAND_SET_BACKLIGHT_NEXT, 0);
		else if (!strcmp(argv[2], "time"))
			ret = set(LFD_COMMAND_SET_RTC_TO_SYSTIME, 0);
		else {
			printf("invalid option\n");
			print_usage(argv[0]);
			return 1;
		}

		if (ret)
			printf("error: %d\n", ret);
	} else {
		printf("invalid option\n");
		print_usage(argv[0]);
		return 1;
	}

	return 0;
}
