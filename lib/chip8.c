#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MEMSIZ 4096

char hexcodes[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

struct machine_t {
	uint8_t mem[MEMSIZ];	//Banco de memoria disponible para la CPU
	uint16_t pc;			//Contador de programa

	uint16_t stack[16];		//Pila, con 16 registros de 16 bits
	uint16_t sp;			//Puntero de pila

	uint8_t v[16];			//16 registros de propósito general
	uint16_t i;				//Registro especial de dirección I
	uint8_t dt, st;			//Temporizadores

	char screen[2048];		//Screen
	char wait_input;		//Tecla de espera
};

void init_machine(struct machine_t* machine) {
	memset(machine, 0x00, sizeof(struct machine_t));
    memcpy(machine->mem + 0x50, hexcodes, 80);
    machine->pc = 0x200;
    machine->wait_input = -1;

	/*machine -> sp = machine -> i = machine -> dt =machine -> st = 0x00;
	machine -> pc = 0x200;

	memset(machine -> mem, 0, MEMSIZ);
	memset(machine -> stack, 0, 32);
	memset(machine -> v, 0, 16);*/
}

void load_rom(struct machine_t* machine) {
	FILE* fp = fopen("INVADERS", "r");

	if(fp == NULL) {
		fprintf(stderr, "ERROR: Cannot open ROM file.\n");
		exit(1);
	}

	//Obtención del tamaño del fichero
	fseek(fp, 0, SEEK_END);
	int length = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	fread(machine -> mem + 0x200, length, 1, fp);

	fclose(fp);
}
