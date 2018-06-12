#include "chip8.c"

#define VERSION "0.1.4"

static void expand(char* from, Uint32* to){
    for(int i = 0; i < 2048; i++) {
        to[i] = (from[i]) ? -1: 0;
    }
}

int main(int argc, const char** argv) {
	struct machine_t mac;

	SDL_AudioDeviceID dev;
	SDL_AudioSpec * spec;
	SDL_Window* win;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	SDL_Surface* surface;
	SDL_Event event;

	int is_running = 1;
	int last_ticks = 0;

	int load_type = 0; //0: load_rom; 1: load_hex;
	char*file;

	//Leer ROM a cargar
	char* rom_file;

	printf("Emu8 - A simple CHIP8 emulator.\nProgrammed by Juan Villacorta.\nVersion %s.\n", VERSION);

	if(argc == 2) {
		load_type = 0;
		file = argv[1];
	} else if(argc == 3 && memcmp(argv[1], "-h", 2) == 0) {
		load_type = 1;
		file = argv[2];
	} else {
		fprintf(stderr, "\nUsage: %s [-h] <ROM file>\n", argv[0]);
		fprintf(stderr, "\n\t-h: if set, will load as hex file.\n\n");
		return 1;
	}

	/*if(argc == 1) {
		fprintf(stderr, "\nUsage: %s <ROM file>\n", argv[0]);
		return 1;
	}*/

	init_machine(&mac);
	if(load_type == 0) {
		if(load_rom(file, &mac)) {
			return 1;
		}
	} else {
		if(load_hex(file, &mac)) {
			return 1;
		}
	}

    SDL_Init(SDL_INIT_EVERYTHING);

    spec = init_audio();
    dev = SDL_OpenAudioDevice(NULL, 0, spec, NULL, SDL_AUDIO_ALLOW_FORMAT_CHANGE);

    if (dev == 0) {
    	fprintf(stderr, "SDL Sound Error: %s\n", SDL_GetError());
    	return 1;
    }

    win = SDL_CreateWindow("Emu8",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        64 * 10, 32 * 10,
        SDL_WINDOW_SHOWN);

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
            cycles = SDL_GetTicks();
        }

        if(SDL_GetTicks() - last_ticks > (1000 / 60)) {
        	if(mac.dt) mac.dt--;
            if(mac.st) {
            	if(--mac.st == 0)
            		SDL_PauseAudioDevice(dev, 1);
            	else
            		SDL_PauseAudioDevice(dev, 0);
            };


            SDL_Delay(0.5);
        	SDL_LockTexture(texture, NULL, &surface -> pixels, &surface -> pitch);
    		expand(mac.screen, (Uint32 *) surface -> pixels);
    		SDL_UnlockTexture(texture);

        	//SDL_RenderClear(renderer);
        	SDL_RenderCopy(renderer, texture, NULL, NULL);
        	SDL_RenderPresent(renderer);
        	last_ticks = SDL_GetTicks();
        }
    }
    printf("\n");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    dispose_audio(spec);
    SDL_CloseAudioDevice(dev);
    SDL_Quit();

	return 0;
}