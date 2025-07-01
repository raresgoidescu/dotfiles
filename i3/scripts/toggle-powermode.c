#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define POWERSAVE "powersave"
#define NORMAL "schedutil"

int main ()
{
	char old_profile[0x100];
	char filename[0x100];
	char* new_profile;
	int n_cpus = sysconf(_SC_NPROCESSORS_ONLN);
	int sg_fd = open("/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor", O_RDONLY);
	if (sg_fd < 0) {
		perror("open");
		exit(-1);
	}

	if (read(sg_fd, old_profile, sizeof(old_profile)) <= 0) {
		perror("read");
		exit(-1);
	}

	close(sg_fd);

	new_profile = strncmp(old_profile, POWERSAVE, sizeof(POWERSAVE) - 1) == 0 ? NORMAL : POWERSAVE;

	for (int cpu = 0; cpu < n_cpus; cpu++) {
		sprintf(filename, "/sys/devices/system/cpu/cpu%d/cpufreq/scaling_governor", cpu);
		sg_fd = open(filename, O_WRONLY);
		if (sg_fd < 0) {
			perror("open");
			exit(-1);
		}

		if (write(sg_fd, new_profile, strlen(new_profile)) != strlen(new_profile)) {
			perror("write");
			exit(-1);
		}

		close(sg_fd);
	}

	setuid(1000);
	char* argv[] = {"/usr/bin/notify-send", new_profile, NULL};
	if (execv(argv[0], argv)) {
		perror("execv");
		exit(-1);
	}
}
