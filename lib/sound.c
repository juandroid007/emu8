#include "sound.h"
#include <math.h>
#include <stdlib.h>

static float tone_pos = 0;
static float tone_inc = 2 * 3.14159 * 1000 / 44000;

static void feed(void* udata, Uint8* stream, int len) {
	for(int i = 0; i < len; i++) {
		stream[i] = sinf(tone_pos) + 127;
		tone_pos += tone_inc;
	}
}

SDL_AudioSpec* init_audio(void) {
	SDL_AudioSpec* spec = (SDL_AudioSpec*) malloc(sizeof(SDL_AudioSpec));
	spec->freq = 44000;
	spec->format = AUDIO_U8;
	spec->channels = 1;
	spec->samples = 4096;
	spec->callback = *feed;
	spec->userdata = NULL;

	return spec;
}

void dispose_audio(SDL_AudioSpec* spec) {
	free(&spec);
}