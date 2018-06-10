#include "cpu.c"
#include <time.h>

static void expand(char* from, Uint32* to){
    for(int i = 0; i < 2048; i++) {
        to[i] = (from[i]) ? -1: 0;
    }
}

int main(int argc, char const *argv[]) {
	struct machine_t mac;

	SDL_Window* win;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	SDL_Surface* surface;
	SDL_Event event;

	int is_running = 1;
	int last_ticks = 0;

	init_machine(&mac);
	load_rom(&mac);

    srand(time(NULL));

    SDL_Init(SDL_INIT_EVERYTHING);

    win = SDL_CreateWindow("Emu8",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        64 * 10, 32 * 10,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        64, 32);

    surface = SDL_CreateRGBSurface(0, 64, 32, 32,
        0x00FF0000,
        0x0000FF00,
        0x000000FF,
        0xFF000000);

    int pitch;
    Uint32* pixels;
    SDL_LockTexture(texture, NULL, &surface -> pixels, &surface -> pitch);

    expand(mac.screen, (Uint32 *) surface -> pixels);
    
    SDL_UnlockTexture(texture);

    int cycles = 0;

    while(is_running) {
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    is_running = 0;
                    break;

            }
        }

        if (SDL_GetTicks() - cycles > 1) {
        	if(mac.wait_input == -1) {
        		step_machine(&mac);
        	} else {
        		for(int key = 0; key <= 0xF; key++) {
        			if(isKeyDown(key)) {
        				mac.v[(int) mac.wait_input] = key;
        				mac.wait_input = -1;
        				break;
        			}
        		}
        	}
            //step_machine(&mac);
            cycles = SDL_GetTicks();
        }

        if(SDL_GetTicks() - last_ticks > (1000 / 60)) {
        	if (mac.dt) mac.dt--;
            if (mac.st) mac.st--;

        	SDL_LockTexture(texture, NULL, &surface -> pixels, &surface -> pitch);
    		expand(mac.screen, (Uint32 *) surface -> pixels);
    		SDL_UnlockTexture(texture);

        	//SDL_RenderClear(renderer);
        	SDL_RenderCopy(renderer, texture, NULL, NULL);
        	SDL_RenderPresent(renderer);
        	last_ticks = SDL_GetTicks();
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();

	return 0;
}