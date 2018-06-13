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

#include <math.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#ifndef SOUND_H_
#define SOUND_H_

#define PI 3.14159

SDL_AudioSpec* init_audio(void);

void dispose_audio(SDL_AudioSpec*);

#endif