#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/sysinfo.h>
#include "colors.h"

struct cpu_t {
	long user, nice, system, idle;
	long total;
	long usage;
};

static int ncpus;
static struct cpu_t prev_total;
static struct cpu_t curr_total;
static struct cpu_t prev_cpus[0x100];  // Do you really have all this cores? can you give me some?
static struct cpu_t curr_cpus[0x100];


static void __attribute__ ((constructor)) get_ncpus() {
	ncpus = get_nprocs();
}

static long calculate_usage(struct cpu_t* prev, struct cpu_t* curr) {
	long diff_idle = curr->idle - prev->idle;
	long diff_total = curr->total - prev->total;
	long diff_usage = diff_total ? (1000 * (diff_total - diff_idle) / diff_total + 5) / 10 : 0;

	return diff_usage;
}

void print_cpu_stat() {
	char buf[0x100];
	int cpu_idx;

	memset(&curr_total, 0, sizeof(struct cpu_t));

	// cpu  2615 1319 2749 92059 816 0 363 0 0 0
	// cpu0 392 266 485 15412 127 0 120 0 0 0
	FILE* stat = fopen("/proc/stat", "r");
	if (stat == NULL) goto cpu_err_pre;

	// skip first line
	if (fgets(buf, sizeof(buf), stat) == NULL) goto cpu_err;
	sscanf(buf, "cpu %ld %ld %ld %ld",
		&curr_total.user,
		&curr_total.nice,
		&curr_total.system,
		&curr_total.idle
	);
	curr_total.total = curr_total.user + curr_total.nice + curr_total.system + curr_total.idle;

	for (int i = 0; i < ncpus; i++) {
		if (fgets(buf, sizeof(buf), stat) == NULL) goto cpu_err;
		if (sscanf(buf, "cpu%d %ld %ld %ld %ld",
			&cpu_idx,
			&curr_cpus[i].user,
			&curr_cpus[i].nice,
			&curr_cpus[i].system,
			&curr_cpus[i].idle
		) != 5) {
			goto cpu_err;
		}
		curr_cpus[i].total = curr_cpus[i].user + curr_cpus[i].nice + curr_cpus[i].system + curr_cpus[i].idle;
		curr_cpus[i].usage = calculate_usage(&curr_cpus[i], &prev_cpus[i]);
	}
	fclose(stat);

	curr_total.usage = calculate_usage(&curr_total, &prev_total);	

	memcpy(&prev_total, &curr_total, sizeof(struct cpu_t));
	memcpy(&prev_cpus, &curr_cpus, ncpus * sizeof(struct cpu_t));

	struct cpu_t* highest_cpu = &curr_cpus[0];
	for (int i = 1; i < ncpus; i++) {
		if (curr_cpus[i].usage > highest_cpu->usage) {
			highest_cpu = &curr_cpus[i];
		}
	}
	
	int temperature = -1;
	FILE* temp = fopen("/sys/class/hwmon/hwmon1/temp1_input", "r");
	if (temp != NULL) {
		char temp_buf[0x20];
		int result = fread(temp_buf, 1, sizeof(temp_buf), temp);
		fclose(temp);
		if (result > 0) {
			temp_buf[result - 1] = '\0';
			temperature = atoi(temp_buf) / 1000;
		}
	}

	if (curr_total.usage >= 90 || highest_cpu->usage >= 90 || temperature >= 90) START_RED;
	else if (curr_total.usage >= 70 || temperature >= 70) START_YELLOW;
	else if (curr_total.usage < 10 || temperature < 40) START_GREEN;
	else START_WHITE;
	printf("CPU ");
	END_COLOR;

	if (curr_total.usage >= 90) START_RED;
	else if (curr_total.usage >= 70) START_YELLOW;
	else if (curr_total.usage < 10) START_GREEN;
	else START_WHITE;
	printf("%ld%% ", curr_total.usage);
	END_COLOR;

	if (highest_cpu->usage >= 90 && curr_total.usage < 70) {
		START_RED;
		printf("%ld%% ", highest_cpu->usage);
		END_COLOR;
	}

	if (temperature >= 90) START_RED;
	else if (temperature >= 70) START_YELLOW;
	else if (temperature < 40) START_GREEN;
	else START_WHITE;
	printf("%d°C", temperature);
	END_COLOR;

	return;

cpu_err:
	fclose(stat);
cpu_err_pre:
	fprintf(stderr, "CPU: %s\n", strerror(errno));
	START_RED;
	printf("CPU error");
	END_COLOR;
}
