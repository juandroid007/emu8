#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MEMSIZ 4096

struct machine_t {
	uint8_t mem[MEMSIZ];	//Banco de memoria disponible para la CPU
	uint16_t pc;			//Contador de programa

	uint16_t stack[16];		//Pila, con 16 registros de 16 bits
	uint16_t sp;			//Puntero de pila

	uint8_t v[16];			//16 registros de propósito general
	uint16_t i;				//Registro especial de dirección I
	uint8_t dt, st;			//Temporizadores
};

void init_machine(struct machine_t* machine) {
	machine -> sp = machine -> i = machine -> dt =machine -> st = 0x00;
	machine -> pc = 0x200;

	for(int i = 0; i < MEMSIZ; i++)
		machine -> mem[i] = 0x00;
	for(int i = 0; i < MEMSIZ; i++) {
		machine -> stack[i] = 0;
		machine -> v[i] = 0;
	}
}

void load_rom(struct machine_t* machine) {
	FILE* fp = fopen("PONG", "r");

	if(fp == NULL) {
		fprintf(stderr, "Cannot open ROM file.\n");
		exit(1);
	}

	//Obtención del tamaño del fichero
	fseek(fp, 0, SEEK_END);
	int length = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	fread(machine -> mem + 0x200, length, 1, fp);

	fclose(fp);
}