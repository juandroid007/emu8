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

		mac.pc += 2;
		if(mac.pc == MEMSIZ)
			mac.pc = 0;

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
				printf("JP %x\n", nnn);
				break;
			case 2: 
				printf("CALL %x\n", nnn);
				break;
			case 3:
				printf("SE %x %x\n", x, kk);
				break;
			case 4:
				printf("SNE %x %x\n", x, kk);
				break;
			case 5:
				printf("SE %x %x\n", x, y);
				break;
			case 6:
				printf("LD %x %x\n", x, kk);
				break;
			case 7:
				printf("ADD %x %x\n", x, kk);
				break;
			case 8:
				switch(n) {
					case 0:
						printf("LD %x %x\n", x, y);
						break;
					case 1:
						printf("OR %x %x\n", x, y);
						break;
					case 2:
						printf("AND %x %x\n", x, y);
						break;
					case 4:
						printf("XOR %x %x\n", x, y);
						break;
					case 5:
						printf("SUB %x %x\n", x, y);
						break;
					case 6:
						printf("SHR %x\n", x);
						break;
					case 7:
						printf("SUBN %x %x\n", x, y);
						break;
					case 0xE:
						printf("SHL %x\n", x);
						break;
				}
			case 9:
				printf("SNE %x %x\n", x, y);
				break;
			case 0xA:
				printf("LD I, %x\n", nnn);
				break;
			case 0xB:
				printf("JP V0, %x\n", nnn);
				break;
			case 0xC:
				printf("RND %x %x\n", x, kk);
				break;
			case 0xD:
				printf("DRW %x %x %x\n", x, y, n);
				break;
			case 0xE:
				if(kk == 0x9E) {
					printf("SKP %x\n", x);
				} else if(kk == 0xA1) {
					printf("SKNP %x\n", x);
				}
				break;
			case 0xF:
				switch(kk) {
					case 0x07:
						printf("LD %x, DT\n", x);
						break;
					case 0x0A:
						printf("LD %x, K\n", x);
						break;
					case 0x15:
						printf("LD DT, %x\n", x);
						break;
					case 0x18:
						printf("LD ST,%x\n", x);
						break;
					case 0x1E:
						printf("ADD I, %x\n", x);
						break;
					case 0x29:
						printf("LD F, %x\n", x);
						break;
					case 0x33:
						printf("LD B,%x\n", x);
						break;
					case 0x55:
						printf("LD [I], %x\n", x);
						break;
					case 0x65:
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