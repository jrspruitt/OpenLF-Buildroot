#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "debug.h"
#include "nor.h"

#define NOR_PARTITION_NAME	"MfgData0"
#define MTD_BASE		"/sys/class/mtd/"

static char found = 0;
static char path[64];

int nor_open(void)
{
	struct dirent *dp;
	DIR *dir;
	char buf[64];
	unsigned int num;
	FILE *f;

	dir = opendir(MTD_BASE);
	if (!dir) {
		dbprintf("can't open " MTD_BASE "\n");
		return 1;
	}

	while ((dp = readdir(dir)) != NULL) {
		if (dp->d_name && dp->d_name[0] != '.') {
			snprintf(path, 63, MTD_BASE "%s/name", dp->d_name);
			f = fopen(path, "r");
			if (!f)
				continue;
			
			if (fscanf(f, "%s", buf) != 1) {
				fclose(f);
				continue;
			}
			
			fclose(f);

			if (!strncmp(buf, NOR_PARTITION_NAME, 63)) {
				if (sscanf(dp->d_name, "mtd%u", &num) != 1)
					break;
				sprintf(path, "/dev/mtdblock%u", num);
				dbprintf("found NOR Flash: \"%s\"\n", path);
				found = 1;
				break;
			}
		}
	}

	closedir(dir);

	return !found;
}

static int nor_read(unsigned int offset, void *dest, size_t size)
{
	FILE *f;
       
	f = fopen(path, "r");
	if (!f) {
		dbprintf("can't open \"%s\"\n", path);
		return 1;
	}

	if (fseek(f, offset, SEEK_SET)) {
		dbprintf("error: invalid offset in NOR Flash\n");
		fclose(f);
		return 2;
	}

	if (fread(dest, size, 1, f) < 0) {
		dbprintf("error: unable to read from NOR Flash\n");
		fclose(f);
		return 3;
	}

	fclose(f);

	return 0;
}

int nor_get_field(enum nor_fields field, void *dest, size_t size)
{
	if (!found)
		return 1;

	switch (field) {
		case NOR_FIELD_BACKLIGHT:
			return nor_read(28, dest, size);
		default:
			return 2;
	}

	return 0;
}
