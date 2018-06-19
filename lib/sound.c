/*Emu8 - A simple CHIP8 emulator.
Copyright (C) 2018  Juan Villacorta

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.*/

#include "sound.h"

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
