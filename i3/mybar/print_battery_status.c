#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "colors.h"

enum {
	UNKNOWN,
	DISCHARGING,
	FULL
};

static int prev_capacity = -1;
static int prev_status = UNKNOWN;

void print_battery_status() {
	int to_read = 2;
	int capacity = -1;
	char status[0x100];
	char command[0x100];
	status[0] = '\0';

	FILE* battery_events = fopen("/sys/class/power_supply/BAT0/uevent", "r");
	if (battery_events == NULL) goto battery_err;


	for (
		char line[0x100];
		to_read > 0 && fgets(line, sizeof(line), battery_events);
		to_read--
	) {
		if (status[0] == '\0' && strncmp(line + strlen("POWER_SUPPLY_"), "STATUS=", strlen("STATUS=")) == 0) {
			strcpy(status, line + strlen("POWER_SUPPLY_STATUS="));
			char* newline = strchr(status, '\n');
			if (newline == NULL) {
				fclose(battery_events);
				goto battery_err;
			}
			*newline = '\0';
		} else
		if (capacity == -1 && strncmp(line + strlen("POWER_SUPPLY_"), "CAPACITY=", strlen("CAPACITY=")) == 0) {
			capacity = atoi(line + strlen("POWER_SUPPLY_CAPACITY="));
		} else {
			to_read++;
			continue;
		}
	}
	fclose(battery_events);

	if (to_read > 0 || capacity == -1 || status[0] == '\0') goto battery_err;

	if (capacity >= 95) START_GREEN;
	else if (capacity <= 10) START_RED;
	else if (capacity <= 20) START_YELLOW;
	else START_WHITE;

	int status_id = strcmp(status, "Discharging") == 0 ? DISCHARGING : strcmp(status, "Full") == 0 ? FULL : UNKNOWN;
	if (status_id == DISCHARGING) {
		printf("BAT %d%%", capacity);
		if (prev_capacity != capacity || prev_status != status_id) {
			prev_capacity = capacity;
			if (capacity <= 10) {
				sprintf(command, "notify-send --urgency=critical --expire-time=10000 'Low battery: %d%%'", capacity);
				system(command);
			}
		}
	} else if (status_id == FULL) {
		printf("FULL %d%%", capacity);
	} else {
		printf("CHR %d%%", capacity);  // My computer is stupid and doesn't understand charging
	}
	END_COLOR;
	prev_capacity = capacity;
	prev_status = status_id;

	return;

battery_err:
	fprintf(stderr, "Battery: %s\n", strerror(errno));
	START_RED;
	printf("Bat error");
	END_COLOR;
}
