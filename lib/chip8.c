#include "cpu.c"
//#include <time.h>

char keys[] = {
    SDL_SCANCODE_X,//0
    SDL_SCANCODE_1,//1
    SDL_SCANCODE_2,//2
    SDL_SCANCODE_3,//3
    SDL_SCANCODE_Q,//4
    SDL_SCANCODE_W,//5
    SDL_SCANCODE_E,//6
    SDL_SCANCODE_A,//7
    SDL_SCANCODE_S,//8
    SDL_SCANCODE_D,//9
    SDL_SCANCODE_Z,//A
    SDL_SCANCODE_C,//B
    SDL_SCANCODE_4,//C
    SDL_SCANCODE_R,//D
    SDL_SCANCODE_F,//E
    SDL_SCANCODE_V //F
};

static int isKeyDown(char key) {
    const Uint8* sdl_keys = SDL_GetKeyboardState(NULL);
    Uint8 pressedKey = keys[(int) key];

    return sdl_keys[pressedKey];
}

void step_machine(struct machine_t* cpu) {
	//srand(time(NULL));

    // Read next opcode from memory.
    uint16_t opcode = (cpu->mem[cpu->pc] << 8) | cpu->mem[cpu->pc + 1];
    cpu->pc = (cpu->pc + 2) & 0xFFF;

    // Extract bit nibbles from the opcode
    uint16_t nnn = opcode & 0x0FFF;
    uint8_t kk = opcode & 0xFF;
    uint8_t n = opcode & 0xF;
    uint8_t x = (opcode >> 8) & 0xF;
    uint8_t y = (opcode >> 4) & 0xF;
    uint8_t p = (opcode >> 12);

    // infernal switch case! (sorry for all the heart attacks here u_u)
    switch (p) {
        case 0:
            if (opcode == 0x00e0) {
                // CLS
                memset(cpu->screen, 0, 2048);
            } else if (opcode == 0x00ee) {
                if (cpu->sp > 0)
                    cpu->pc = cpu->stack[--cpu->sp];
            }
            break;
        case 1:
            // jp nnn: set program counter to nnn
            cpu->pc = nnn;
            break;
        case 2:
            // call nnn: stack[sp++] = pc, pc = nnn
            if (cpu->sp < 16)
                cpu->stack[cpu->sp++] = cpu->pc;
            cpu->pc = nnn;
            break;
        case 3:
            // se x, kk: if v[x] == kk -> pc += 2
            if (cpu->v[x] == kk)
                cpu->pc = (cpu->pc + 2) & 0xfff;
            break;
        case 4:
            // sne x, kk: if v[x] != kk -> pc += 2
            if (cpu->v[x] != kk)
                cpu->pc = (cpu->pc + 2) & 0xfff;
            break;
        case 5:
            // se x, y: if v[x] == v[y] -> pc += 2
            if (cpu->v[x] == cpu->v[y])
                cpu->pc = (cpu->pc + 2) & 0xfff;
            break;
        case 6:
            // ld x, kk: v[x] = kk
            cpu->v[x] = kk;
            break;
        case 7:
            // add x, kk: v[x] = (v[x] + kk) & 0xff
            cpu->v[x] = (cpu->v[x] + kk) & 0xff;
            break;
        case 8:
            switch (n) {
                case 0:
                    // ld x, y: v[x] = v[y]
                    cpu->v[x] = cpu->v[y];
                    break;
                case 1:
                    // or x, y: v[x] = v[x] | v[y];
                    cpu->v[x] |= cpu->v[y];
                    break;
                case 2:
                    // and x, y: v[x] = v[x] & v[y]
                    cpu->v[x] &= cpu->v[y];
                    break;
                case 3:
                    // xor x, y: v[x] = v[x] ^ v[y]
                    cpu->v[x] ^= cpu->v[y];
                    break;
                case 4:
                    // add x, y: v[x] += v[y]
                    cpu->v[0xf] = (cpu->v[x] > cpu->v[x] + cpu->v[y]);
                    cpu->v[x] += cpu->v[y];
                    break;
                case 5:
                    // SUB x, y: V[x] -= V[y]
                    cpu->v[0xF] = (cpu->v[x] > cpu->v[y]);
                    cpu->v[x] -= cpu->v[y];
                    break;
                case 6:
                    // SHR x : V[x] = V[x] >> 1
                    cpu->v[0xF] = (cpu->v[x] & 1);
                    cpu->v[x] >>= 1;
                    break;
                case 7:
                    // SUBN x, y: V[x] = V[y] - V[x]
                    cpu->v[0xF] = (cpu->v[y] > cpu->v[x]);
                    cpu->v[x] = cpu->v[y] - cpu->v[x];
                    break;
                case 0xE:
                    // SHL x : V[x] = V[x] << 1
                    cpu->v[0xF] = ((cpu->v[x] & 0x80) != 0);
                    cpu->v[x] <<= 1;
                    break;
            }
            break;
        case 9:
            // SNE x, y: V[x] != V[y] -> pc += 2;
            if (cpu->v[x] != cpu->v[y])
                cpu->pc = (cpu->pc + 2) & 0xFFF;
            break;
        case 0xA:
            // LD I, x : I = nnn
            cpu->i = nnn;
            break;
        case 0xB:
            // JP V0, nnn: pc = V[0] + nnn
            cpu->pc = (cpu->v[0] + nnn) & 0xFFF;
            break;
        case 0xC:
            // RND x, kk: V[x] = random() % kk
            cpu->v[x] = rand() & kk;
            break;
        case 0xD:
            /*
             * DRW x, y, n:
             * Dibuja un sprite en el pixel v[x], v[y].
             * El número de filas a dibujar se dice con n.
             * El sprite se saca de la dirección de memoria [I].
             */
            cpu->v[15] = 0;
            for (int j = 0; j < n; j++) {
                uint8_t sprite = cpu->mem[cpu->i + j];
                for (int i = 0; i < 8; i++) {
                    int px = (cpu->v[x] + i) & 63;
                    int py = (cpu->v[y] + j) & 31;
                    int pos = 64 * py + px;
                    int pixel = (sprite & (1 << (7-i))) != 0;
                    
                    cpu->v[15] |= (cpu->screen[pos] & pixel);
                    cpu->screen[pos] ^= pixel;
                }
            }
            break;
        case 0xE:
            if (kk == 0x9E) {
                //SKP x: if key V[x] isDown is true, skip next instruction
                if(isKeyDown(cpu->v[x])) {
                    cpu->pc = (cpu->pc + 2) & 0xFFF;
                }
            } else if (kk == 0xA1) {
                if(!isKeyDown(cpu->v[x])) {
                    cpu->pc = (cpu->pc + 2) & 0xFFF;
                }
            }
            break;
        case 0xF:
            switch (kk) {
                case 0x07:
                    // LD V[x], DT: V[x] = DT
                    cpu->v[x] = cpu->dt;
                    break;
                case 0x0A:
                    //LD X, J: wait input key
                    cpu->wait_input = x;
                    break;
                case 0x15:
                    // LD DT, V[x] -> DT = V[x]
                    cpu->dt = cpu->v[x];
                    break;
                case 0x18:
                    // LD ST, V[x] -> ST = V[x]
                    cpu->st = cpu->v[x];
                    break;
                case 0x1E:
                    // ADD I, V[x] -> I += V[x]
                    cpu->i += cpu->v[x];
                    break;
                case 0x29:
                    // LD F, V[x] -> I = [posicion de memoria del número V[x]]
                    cpu->i = 0x50 + (cpu->v[x] & 0xF) * 5;
                    break;
                case 0x33:
                    // LD B, V[x] = loads BCD number in memory
                    cpu->mem[cpu->i + 2] = cpu->v[x] % 10;
                    cpu->mem[cpu->i + 1] = (cpu->v[x] / 10) % 10;
                    cpu->mem[cpu->i] = (cpu->v[x] / 100); 
                    break;
                case 0x55:
                    // LD [I], X -> guarda en I
                    for (int reg = 0; reg <= x; reg++)
                        cpu->mem[cpu->i + reg] = cpu->v[reg];
                    break;
                case 0x65:
                    // LD X, [I] -> lee de I
                    for (int reg = 0; reg <= x; reg++)
                        cpu->v[reg] = cpu->mem[cpu->i + reg];
                    break;
            }
            break;
    }
}