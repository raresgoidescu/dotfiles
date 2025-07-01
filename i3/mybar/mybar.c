#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <assert.h>
#include <sys/time.h>
#include "colors.h"


extern void print_datetime(int primary);
extern void print_microphone_volume();
extern void print_volume();
extern void print_battery_status();
extern void print_disk_space();
extern void print_used_memory();
extern void print_cpu_stat();
extern void print_internet_status();

extern void update_microphone_volume();
void handler(int signal) {
}


int main (int argc, char** argv) {
	assert(argc == 2);
	int primary = strcmp(argv[1], "primary") == 0;
	if (!primary) assert(strcmp(argv[1], "nonprimary") == 0);

	signal(SIGUSR1, handler);

	printf("{\"version\":1}\n[\n");
	while (1) {
		printf("[");
		if (primary) {
			print_internet_status();
			print_cpu_stat(); print_separator;
			print_used_memory(); print_separator;
			print_disk_space(); print_separator;
			print_battery_status(); print_separator;
			print_volume(); print_separator;
			print_microphone_volume(); print_separator;
		}
		print_datetime(primary); if (primary) { print_separator_final; }

		printf("],\n");
		fflush(stdout);

		int interval = 1;
		struct timeval current_timeval;
		gettimeofday(&current_timeval, NULL);
		struct timespec ts = {interval - 1 - (current_timeval.tv_sec % interval), (10e5 - current_timeval.tv_usec) * 1000};
		nanosleep(&ts, NULL);
	}
}
