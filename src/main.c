#include <SDL2/SDL.h>
#include "chip8.c"

int main(int argc, const char * argv[]) {
	struct machine_t mac;

	init_machine(&mac);
	load_rom(&mac);

	int damnQuit = 0;

	while(!damnQuit) {
		//Leer opcode
		uint16_t opcode = (mac.mem[mac.pc] << 8) | mac.mem[mac.pc + 1];

		mac.pc = (mac.pc + 2) & (MEMSIZ - 1);

		//mac.pc += 2;
		/*if(mac.pc == MEMSIZ)
			mac.pc = 0;*/

		uint16_t nnn = opcode & 0x0FFF;
		uint8_t kk = opcode & 0xFF;
		uint8_t n = opcode & 0xF;
		uint8_t x = (opcode >> 8) & 0xF;
		uint8_t y = (opcode >> 4) & 0xF;
		uint8_t p = (opcode >> 12);

		switch(p) {
			case 0:
				if(opcode == 0x00E0) {
					printf("CLS\n");
				} else if(opcode == 0x00EE) {
					printf("RET\n");
				}
				break;
			case 1:
				//JP. Coloca el pc en nnn
				mac.pc == nnn;
				printf("JP %x\n", nnn);
				break;
			case 2: 
				printf("CALL %x\n", nnn);
				break;
			case 3:
				//SE. if V[x] == kk -> pc += 2
				if(mac.v[x] == kk)
					mac.pc = (mac.pc + 2) & 0xFFF;
				printf("SE %x %x\n", x, kk);
				break;
			case 4:
				//SNE. if V[x] != kk -> pc += 2
				if(mac.v[x] != kk)
					mac.pc = (mac.pc + 2) & 0xFFF;
				printf("SNE %x %x\n", x, kk);
				break;
			case 5:
				//SE x, y. if V[x] != V[y] -> pc += 2
				if(mac.v[x] != mac.v[y])
					mac.pc = (mac.pc + 2) & 0xFFF;
				printf("SE %x %x\n", x, y);
				break;
			case 6:
				//LD x. V[x] == kk
				mac.v[x] = kk;
				printf("LD %x %x\n", x, kk);
				break;
			case 7:
				//ADD x, kk. V[x] = (V[x] + kk) & 0xFF
				mac.v[x] = (mac.v[x] = kk) & 0xFF;
				printf("ADD %x %x\n", x, kk);
				break;
			case 8:
				switch(n) {
					case 0:
						//LD x, y. V[x] = V[y]
						mac.v[x] = mac.v[y];
						printf("LD %x %x\n", x, y);
						break;
					case 1:
						//OR x, y. V[x] = V[x] | V[y]
						mac.v[x] |= mac.v[y];
						printf("OR %x %x\n", x, y);
						break;
					case 2:
						//AND x, y. V[x] = V[x] & V[y]
						mac.v[x] &= mac.v[y];
						printf("AND %x %x\n", x, y);
						break;
					case 3:
						//XOR x, y. V[x] = V[x] ^ V[y]
						mac.v[x] ^= mac.v[y];
						printf("XOR %x %x\n", x, y);
						break;
					case 4:
						//ADD x, y. V[x] = V[x] += V[y] & 0xFF
						mac.v[0xF] = (mac.v[x] > mac.v[x] + mac.v[y]);
						mac.v[x] += mac.v[y];
						printf("ADD %x %x\n", x, y);
						break;
					case 5:
						//SUB x, y. V[x] = V[x] -= V[y]
						mac.v[0xF] = (mac.v[x] > mac.v[y]);
						mac.v[x] -= mac.v[y];
						printf("SUB %x %x\n", x, y);
						break;
					case 6:
						//SHR x. V[x] = V[x] >> 1
						mac.v[0xF] = (mac.v[x] & 0x01);
						mac.v[x] >>= 1;
						printf("SHR %x\n", x);
						break;
					case 7:
						//SUB y, x. V[y] = V[xy -= V[x]
						mac.v[0xF] = (mac.v[y] > mac.v[x]);
						mac.v[y] -= mac.v[x];
						printf("SUBN %x %x\n", x, y);
						break;
					case 0xE:
						//SHR x. V[x] = V[x] << 1
						mac.v[0xF] = ((mac.v[x] & 0x80) != 0);
						mac.v[x] <<= 1;
						printf("SHL %x\n", x);
						break;
				}
			case 9:
				//SNE x, y. V[x] != V[y] -> pc += 2
				if(mac.v[x] != mac.v[y])
					mac.pc = (mac.pc +2) & 0xFFF;
				printf("SNE %x %x\n", x, y);
				break;
			case 0xA:
				//LD I, nnn. I = nnn
				mac.i = nnn; 
				printf("LD I, %x\n", nnn);
				break;
			case 0xB:
				//JP V0, nnn. pc = V[0] + nnn
				mac.pc = mac.v[0] + nnn; 
				printf("JP V0, %x\n", nnn);
				break;
			case 0xC:
				//TODO
				printf("RND %x %x\n", x, kk);
				break;
			case 0xD:
				printf("DRW %x %x %x\n", x, y, n);
				break;
			case 0xE:
				//TODO
				if(kk == 0x9E) {
					printf("SKP %x\n", x);
				} else if(kk == 0xA1) {
					printf("SKNP %x\n", x);
				}
				break;
			case 0xF:
				switch(kk) {
					case 0x07:
						//LD V[x], DT. V[x] = DT
						mac.v[x] = mac.dt;
						printf("LD %x, DT\n", x);
						break;
					case 0x0A:
						//TODO
						printf("LD %x, K\n", x);
						break;
					case 0x15:
						//LD DY, V[x] -> DT = V[x]
						mac.dt = mac.v[x];
						printf("LD DT, %x\n", x);
						break;
					case 0x18:
						//LD, ST. V[x] -> ST = V[x]
						//TODO
						mac.st = mac.v[x];
						printf("LD ST,%x\n", x);
						break;
					case 0x1E:
						//ADD I, V[x] -> I += V[x]
						mac.i += mac.v[x];
						printf("ADD I, %x\n", x);
						break;
					case 0x29:
						//TODO
						printf("LD F, %x\n", x);
						break;
					case 0x33:
						//TODO
						printf("LD B,%x\n", x);
						break;
					case 0x55:
						//TODO
						printf("LD [I], %x\n", x);
						break;
					case 0x65:
						//TODO
						printf("LD %x, [I]\n", x);
						break;
				}
				break;
		}
	}

	//printf("%x\n\n", mac.mem[0x200]);
	printf("\nSucces.\n");

	return 0;
}