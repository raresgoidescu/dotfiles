#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <linux/wireless.h>
#include "colors.h"


#define IW_INTERFACE "wlan0"
#define ETH_INTERFACE "enp1s0"

static double prev_time;
static unsigned long piw_dw, piw_up, peth_dw, peth_up;
static double iw_dw_rate, iw_up_rate;
static double eth_dw_rate, eth_up_rate;

static unsigned long last_time_iw_dw, last_time_iw_up;
static unsigned long last_time_eth_dw, last_time_eth_up;

static double get_time() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + (double)tv.tv_usec / 1e6;	
}

static void get_up_down() {
	char line[0x100];

	// Inter-|   Receive                                                |  Transmit
	// face |bytes    packets errs drop fifo frame compressed multicast|bytes    packets errs drop fifo colls carrier compressed
	// lo:  102344     945    0    0    0     0          0         0   102344     945    0    0    0     0       0          0
	FILE* proc_net_dev = fopen("/proc/net/dev", "r");
	if (proc_net_dev == NULL) {
		fprintf(stderr, "inet: %s\n", strerror(errno));
		return;
	}
	double curr_time = get_time();

	// Ignore first two lines
	if (fgets(line, sizeof(line), proc_net_dev) == NULL) { fclose(proc_net_dev); return; };
	if (fgets(line, sizeof(line), proc_net_dev) == NULL) { fclose(proc_net_dev); return; };

	char interface[0x50];
	unsigned long rbts, rpkts, rerrs, rdrop, rfifo, rframe, rcompressed, rmulticase;
	unsigned long sbts, spkts, serrs, sdrop, sfifo, sframe, scompressed, smulticase;
	unsigned long iw_dw = 0, iw_up = 0, eth_dw = 0, eth_up = 0;

	while (fscanf(
		proc_net_dev,
		"%s %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
		interface,
		&rbts, &rpkts, &rerrs, &rdrop, &rfifo, &rframe, &rcompressed, &rmulticase, 
		&sbts, &spkts, &serrs, &sdrop, &sfifo, &sframe, &scompressed, &smulticase
	) == 17) {
		if (strncmp(interface, IW_INTERFACE, strlen(IW_INTERFACE)) == 0) {
			iw_dw = rbts;
			iw_up = sbts;
		} else if (strncmp(interface, ETH_INTERFACE, strlen(ETH_INTERFACE)) == 0) {
			eth_dw = rbts;
			eth_up = sbts;
		}	
	}

	if (prev_time == 0) {
		iw_dw_rate = 0;
		iw_up_rate = 0;
		eth_dw_rate = 0;
		eth_up_rate = 0;
	} else {
		double time_diff = curr_time - prev_time;
		double iw_dw_diff = iw_dw - piw_dw;
		double iw_up_diff = iw_up - piw_up;
		double eth_dw_diff = eth_dw - peth_dw;
		double eth_up_diff = eth_up - peth_up;

		// B/s
		iw_dw_rate = iw_dw_diff / time_diff;
		iw_up_rate = iw_up_diff / time_diff;
		eth_dw_rate = eth_dw_diff / time_diff;
		eth_up_rate = eth_up_diff / time_diff;
	}

	piw_dw = iw_dw;
	piw_up = iw_up;
	peth_dw = eth_dw;
	peth_up = eth_up;
	prev_time = curr_time;

	fclose(proc_net_dev);
}

static char* get_wireless_essid() {
	struct iwreq wreq;
	memset(&wreq, 0, sizeof(struct iwreq));
	strcpy(wreq.ifr_name, IW_INTERFACE);

	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd == -1) return 0;

	char *essid = calloc(sizeof(char), IW_ESSID_MAX_SIZE+1);
	wreq.u.essid.pointer = essid;
	wreq.u.essid.length = IW_ESSID_MAX_SIZE;
	if (ioctl(sockfd, SIOCGIWESSID, &wreq)) {
		close(sockfd);
		free(essid);
		return NULL;
	}
	close(sockfd);

	if (essid[0] == '\0') {
		free(essid);
		return NULL;
	}

	return essid;
}

static int print_wireless_status() {
	char* essid = get_wireless_essid();
	int no_essid = 0;
	if (essid == NULL) {
		no_essid = 1;
		essid = "Unknown";
	}

	if (no_essid) {
		return 0;
	} else {
		START_WHITE;
		printf("W: %s", essid);

		if (piw_dw > 0) {
			double dw_rate = iw_dw_rate / (1024 * 1024);
			double up_rate = iw_up_rate / (1024 * 1024);

			if (dw_rate > 0.01 || last_time_iw_dw > prev_time - 10) {  // prevent showing speed only every other second in case of low usage
				printf(" ↓ %.2F MiB/s", dw_rate);
				if (dw_rate > 0.01) last_time_iw_dw = prev_time;
			}
			if (up_rate > 0.01 || last_time_iw_dw > prev_time - 10) {
				printf(" ↑ %.2F MiB/s", up_rate);
				if (up_rate > 0.01) last_time_iw_up = prev_time;
			}
		}
		END_COLOR;

		print_separator;

		free(essid);
		return 1;
	}

}

static const char *get_ip_addr(const char *interface, int family) {
    static char part[512];
    socklen_t len = 0;

    if (family == AF_INET) len = sizeof(struct sockaddr_in);
    else if (family == AF_INET6) len = sizeof(struct sockaddr_in6);

    memset(part, 0, sizeof(part));

    struct ifaddrs *ifaddr, *addrp;
    getifaddrs(&ifaddr);

    if (ifaddr == NULL) return NULL;

    for (addrp = ifaddr; addrp != NULL; addrp = addrp->ifa_next) {
        char *stripped_ifa_name = strdup(addrp->ifa_name);
		if (stripped_ifa_name == NULL) continue;
		char* ptr = strchr(stripped_ifa_name, ':');
		if (ptr) *ptr = '\0';

        int name_matches = strcmp(stripped_ifa_name, interface) != 0;
        free(stripped_ifa_name);
        if (name_matches) continue;

        if (addrp->ifa_addr != NULL && addrp->ifa_addr->sa_family == family) break;

        if ((addrp->ifa_flags & IFF_RUNNING) == 0) {
            freeifaddrs(ifaddr);
            return NULL;
        }
    }

    if (addrp == NULL) {
        freeifaddrs(ifaddr);
		return NULL;
    }

    if (getnameinfo(addrp->ifa_addr, len, part, sizeof(part), NULL, 0, NI_NUMERICHOST) != 0) {
        fprintf(stderr, "inet: %s\n", strerror(errno));
        freeifaddrs(ifaddr);
        return NULL;
    }

    freeifaddrs(ifaddr);
    return part;
}

static char* sstrdup(const char* str) {
	return str == NULL ? NULL : strdup(str);
}

static int print_ethernet_status() {
	char* ipv4 = sstrdup(get_ip_addr(ETH_INTERFACE, AF_INET));
	char* ipv6 = NULL;
	char* ip_address = ipv4;

	if (ipv4 == NULL) {
		ipv6 = sstrdup(get_ip_addr(ETH_INTERFACE, AF_INET6));
		if (ipv6 == NULL) return 0;
		char* sep = strchr(ipv6, '%');
		if (sep) *sep = '\0';
		ip_address = ipv6;
	}

	START_WHITE;
	printf("E: %s", ip_address);

	if (peth_dw > 0) {
		double dw_rate = eth_dw_rate / (1024 * 1024);
		double up_rate = eth_up_rate / (1024 * 1024);

		if (dw_rate > 0.01 || last_time_eth_dw > prev_time - 10) {
			printf(" ↓ %.2F MiB/s", dw_rate);
			if (dw_rate > 0.01) last_time_eth_dw = prev_time;
		}
		if (up_rate > 0.01 || last_time_eth_up > prev_time - 10) {
			printf(" ↑ %.2F MiB/s", up_rate);
			if (up_rate > 0.01) last_time_eth_up = prev_time;
		}
	}

	END_COLOR;

	print_separator;

	if (ipv4) free(ipv4);
	if (ipv6) free(ipv6);

	return 1;
}

void print_internet_status() {
	get_up_down();
	int ethernet = print_ethernet_status();
	int wireless = print_wireless_status();

	if (!ethernet && !wireless) {
		START_RED;
		printf("No connection");
		END_COLOR;
		print_separator;
	}
}
