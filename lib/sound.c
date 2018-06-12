#include "sound.h"
#include <math.h>
#include <stdlib.h>

struct audiodata_t {
	float tone_pos;
	float tone_inc;
};

static void feed(void* udata, Uint8* stream, int len) {
	struct audiodata_t* audio = (struct audiodata_t *) udata;
	for(int i = 0; i < len; i++) {
		stream[i] = sinf(audio->tone_pos) + 127;
		audio->tone_pos += audio->tone_inc;
	}
}

SDL_AudioSpec* init_audio(void) {
	struct audiodata_t* audio = malloc(sizeof(struct audiodata_t));
	audio->tone_pos = 0;
	audio->tone_inc = 2 * PI * 1000 / 44000;

	SDL_AudioSpec* spec = (SDL_AudioSpec*) malloc(sizeof(SDL_AudioSpec));
	spec->freq = 44000;
	spec->format = AUDIO_U8;
	spec->channels = 1;
	spec->samples = 4096;
	spec->callback = *feed;
	spec->userdata = audio;

	return spec;
}

void dispose_audio(SDL_AudioSpec* spec) {
	free(spec->userdata);
	//free(&spec);
}