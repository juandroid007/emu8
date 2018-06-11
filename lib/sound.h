#include <SDL2/SDL.h>

#ifndef SOUND_H_
#define SOUND_H_

SDL_AudioSpec* init_audio(void);

void dispose_audio(SDL_AudioSpec*);

#endif