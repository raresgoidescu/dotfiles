#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "colors.h"

void print_used_memory() {
	int to_read = 8;
	unsigned long memtotal, memfree, memavailable, membuffer, memcache, memshared, swaptotal, swapfree;
	memtotal = -1UL, memfree = -1UL, memavailable = -1UL, membuffer = -1UL, memcache = -1UL, memshared = -1UL, swaptotal = -1UL, swapfree = -1UL;

	FILE* meminfo = fopen("/proc/meminfo", "r");
	if (meminfo == NULL) goto memory_err;

	for(
		char line[0x100];
		to_read > 0 && fgets(line, sizeof(line), meminfo);
		to_read--
	) {
		if (memtotal == -1UL && strncmp(line, "MemTotal:", strlen("MemTotal:")) == 0) {
			memtotal = strtoul(line + strlen("MemTotal:"), NULL, 10);
		} else
		if (memfree == -1UL && strncmp(line, "MemFree:", strlen("MemFree:")) == 0) {
			memfree = strtoul(line + strlen("MemFree:"), NULL, 10);
		} else
		if (memavailable == -1UL && strncmp(line, "MemAvailable:", strlen("MemAvailable")) == 0) {
			memavailable = strtoul(line + strlen("MemAvailable:"), NULL, 10);
		} else
		if (membuffer == -1UL && strncmp(line, "Buffers:", strlen("Buffers:")) == 0) {
			membuffer = strtoul(line + strlen("Buffers:"), NULL, 10);
		} else
		if (memcache == -1UL && strncmp(line, "Cached:", strlen("Cached:")) == 0) {
			memcache = strtoul(line + strlen("Cached:"), NULL, 10);
		} else
		if (swaptotal == -1UL && strncmp(line, "SwapTotal:", strlen("SwapTotal:")) == 0) {
			swaptotal = strtoul(line + strlen("SwapTotal:"), NULL, 10);
		} else
		if (swapfree == -1UL && strncmp(line, "SwapFree:", strlen("SwapFree")) == 0) {
			swapfree = strtoul(line + strlen("SwapFree:"), NULL, 10);
		} else
		if (memshared == -1UL && strncmp(line, "Shmem:", strlen("Shmem")) == 0) {
			memshared = strtoul(line + strlen("Shmem:"), NULL, 10);
		} else {
			to_read++;
			continue;
		}
	}
	fclose(meminfo);

	if (to_read > 0 || memtotal == -1UL || memfree == -1UL || memavailable == -1UL || membuffer == -1UL || memcache == -1UL || swaptotal == -1ULL || swapfree == -1UL || memshared == -1UL) goto memory_err;
	
	memtotal *= 1024;
	memfree *= 1024;
	memavailable *= 1024;
	membuffer *= 1024;
	memcache *= 1024;
	swaptotal *= 1024;
	swapfree *= 1024;
	memshared *= 1024;

	unsigned long memused = memtotal - memfree - membuffer - memcache;
	unsigned long swapused = swaptotal - swapfree;

	if (memused >= 6UL * 1024 * 1024 * 1024 || swapused >= 5UL * 1024 * 1024 * 1024 / 2) START_RED;
	else if (memused >= 5UL * 1024 * 1024 * 1024) START_YELLOW;
	else if (memused < 3UL * 1024 * 1024 * 1024 / 2) START_GREEN;
	else START_WHITE;
	printf("Mem ");
	END_COLOR;

	if (memused >= 6UL * 1024 * 1024 * 1024) START_RED;
	else if (memused >= 5UL * 1024 * 1024 * 1024) START_YELLOW;
	else if (memused < 3UL * 1024 * 1024 * 1024 / 2) START_GREEN;
	else START_WHITE;
	if (memused > 1UL * 1024 * 1024 * 1024) {
		printf("%.1F GiB", (double)memused / 1024 / 1024 / 1024);
	} else {
		printf("%.1F MiB", (double)memused / 1024 / 1024);
	}
	END_COLOR;

	if (swapused >= 1024UL * 1024 * 1024) {
		if (swapused >= 2UL * 1024 * 1024 * 1024) START_RED;
		else START_WHITE;
		printf(" Swap %.1F GiB", (double)swapused / 1024 / 1024 / 1024);
		END_COLOR;
	}

	return;

memory_err:
	fprintf(stderr, "Memory: %s\n", strerror(errno));
	START_RED;
	printf("Mem error");
	END_COLOR;
}
