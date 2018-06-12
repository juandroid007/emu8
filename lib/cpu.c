#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <SDL2/SDL.h>

char hex_to_bin(char hex) {
    if(hex >= '0' && hex <= '9')
        return hex - '0';
    hex &= 0xDF;
    if(hex >= 'A' && hex <= 'F')
        return 10 + (hex - 'A');
    return -1;
}

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

int load_hex(const char* file, struct machine_t* machine) {
    FILE* fp = fopen(file, "rb");
    
    if (fp == NULL) {
        fprintf(stderr, "Cannot open ROM file.\n");
        return 1;
    }

    // Use the fseek/ftell/fseek trick to retrieve file size.
    fseek(fp, 0, SEEK_END);
    int length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    // Create a temporal buffer where to store the data.
    char* hexfile = malloc(length);
    if (hexfile == NULL) {
        return 1;
    }

    fread(hexfile, length, 1, fp);
    fclose(fp);

    int mempos = 0x200;
    
    if (length & 0x01) length--;
    for (int i = 0; i < length; i += 2)
    {
        char hi = hexfile[i];
        char lo = hexfile[i + 1];

        char hi_b = hex_to_bin(hi);
        char lo_b = hex_to_bin(lo);
        if (hi_b == -1 || lo_b == -1) {
            free(hexfile);
            return 1;
        }

        machine->mem[mempos++] = hi_b << 4 | lo_b;   
        if (mempos > 0xFFF)
            break;
    }

    free(hexfile);

    return 0;
}