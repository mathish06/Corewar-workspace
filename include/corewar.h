/*
** EPITECH PROJECT, 2026
** corewar.h
** File description:
** the .h for corewar
*/

#ifndef COREWAR_H_
    #define COREWAR_H_

    #include "./op.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <stdint.h>

    #define MEM_SIZE (6 * 1024)

typedef struct champion_s {
    header_t header;
    uint8_t *code;
    int load_address;
    int prog_number;
} champion_t;

typedef struct vm_s {
    uint8_t arena[MEM_SIZE];
} vm_t;

int load_champion(champion_t *champ, const char *filepath);
void free_champion(champion_t *champ);
int swap_int32(int val);

void init_vm(vm_t *vm);
void dump_arena(const vm_t *vm);

#endif
