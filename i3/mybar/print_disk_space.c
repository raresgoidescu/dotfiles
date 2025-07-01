#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/statvfs.h>
#include "colors.h"

void print_disk_space() {
	struct statvfs stat;

	if (statvfs("/", &stat)) goto disk_err;

	unsigned long free_space = stat.f_bsize * stat.f_bavail;

	if (free_space < 5UL * 1024 * 1024 * 1024) START_RED;
	else if (free_space < 15UL * 1024 * 1024 * 1024) START_YELLOW;
	else if (free_space > 60UL * 1024 * 1024 * 1024) START_GREEN;
	else START_WHITE;

	if (free_space < 1024 * 1024 * 1024) {
		printf("Disk %.1F MiB", ((double)free_space) / 1024 / 1024);
	} else {
		printf("Disk %.1F GiB", ((double)free_space) / 1024 / 1024 / 1024);
	}
	
	END_COLOR;

	return;

disk_err:
	fprintf(stderr, "Disk: %s\n", strerror(errno));
	START_RED;
	printf("Disk error");
	END_COLOR;
}
