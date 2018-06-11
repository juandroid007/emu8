#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <SDL2/SDL.h>

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

int load_rom(const char* file, struct machine_t* machine) {
    FILE* fp = fopen(file, "rb");

    if(fp == NULL) {
        fprintf(stderr, "\nERROR: Cannot open ROM file.\n");
        return 1;
    }

    //Obtención del tamaño del fichero
    fseek(fp, 0, SEEK_END);
    int length = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if(length > 3584) {
        fprintf(stderr, "\nERROR: ROM too large.\n");
        return 1;
    }

    fread(machine -> mem + 0x200, length, 1, fp);

    fclose(fp);

    return 0;
}