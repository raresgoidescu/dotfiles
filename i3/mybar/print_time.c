#include <stdio.h>
#include <time.h>
#include "colors.h"

const char* week_day[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

void print_datetime(int primary) {
	time_t t = time(NULL);
	struct tm* tm = localtime(&t);
	START_WHITE;
	printf("%s %02d-%02d-%04d %d:%02d:%02d",
		week_day[tm->tm_wday],
		tm->tm_mday,
		tm->tm_mon + 1,
		tm->tm_year + 1900,
		tm->tm_hour,
		tm->tm_min,
		tm->tm_sec
	);
	if (primary) {
		END_COLOR;
	} else {
		printf(" ");
		END_COLOR_FINAL;
	}
}
