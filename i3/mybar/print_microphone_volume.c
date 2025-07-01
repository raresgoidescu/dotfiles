#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <errno.h>
#include <alsa/asoundlib.h>
#include "colors.h"

void print_microphone_volume() {
    const long MAX_LINEAR_DB_SCALE = 24;
    snd_mixer_t *mixer;
    snd_mixer_selem_id_t *sid;
    snd_mixer_elem_t *elem;
    long min, max, val;
    int force_linear = 0;
    int avg;
	int pbval = 1;

	if (snd_mixer_open(&mixer, 0) < 0) goto volume_err;
	if (snd_mixer_attach(mixer, "default") < 0) goto volume_err;
	if (snd_mixer_selem_register(mixer, NULL, NULL) < 0) goto volume_err;
	if (snd_mixer_load(mixer) < 0) goto volume_err;

	snd_mixer_selem_id_alloca(&sid);
	if (sid == NULL) {
		snd_mixer_close(mixer);
		goto volume_err;
	}

	snd_mixer_selem_id_set_index(sid, 0);
	snd_mixer_selem_id_set_name(sid, "Capture");

	elem = snd_mixer_find_selem(mixer, sid);
	if (elem == NULL) goto volume_err_late;

	snd_mixer_handle_events(mixer);
	int res = snd_mixer_selem_get_capture_dB_range(elem, &min, &max) || snd_mixer_selem_get_capture_dB(elem, 0, &val);
	if (res || min >= max) {
		res = snd_mixer_selem_get_capture_volume_range(elem, &min, &max);
		res |= snd_mixer_selem_get_capture_volume(elem, 0, &val);
		force_linear = 1;
	}

	if (res != 0) goto volume_err_late;

	if (force_linear || max - min <= MAX_LINEAR_DB_SCALE * 100) {
		float avgf = ((float)(val - min) / (max - min)) * 100;
		avg = (int)avgf;
		avg = (avgf - avg < 0.5 ? avg : (avg + 1));
	} else {
		double normalized = exp10((val - max) / 6000.0);
		if (min != SND_CTL_TLV_DB_GAIN_MUTE) {
			double min_norm = exp10((min - max) / 6000.0);
			normalized = (normalized - min_norm) / (1 - min_norm);
		}
		avg = lround(normalized * 100);
	}

	if (snd_mixer_selem_get_capture_switch(elem, 0, &pbval) < 0) goto volume_err_late;

	if (pbval == 0) {
		START_YELLOW;
		printf("Mic muted");
		END_COLOR;
	} else {
		if (avg < 10) START_YELLOW;
		else if (avg > 100) START_RED;
		else if (avg == 100) START_YELLOW;
		else START_WHITE;

		printf("Mic %d%%", avg);

		END_COLOR;
	}

	snd_mixer_close(mixer);

	return;

volume_err_late:
	snd_mixer_close(mixer);
volume_err:
	fprintf(stderr, "Mic: %s\n", strerror(errno));
	START_RED;
	printf("Mic error");
	END_COLOR;
}

