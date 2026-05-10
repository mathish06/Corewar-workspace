/*
** EPITECH PROJECT, 2026
** arena.c
** File description:
** Arena initialization and dump
*/

#include "../include/corewar.h"

void init_vm(vm_t *vm)
{
    if (vm == NULL) {
        return;
    }
    for (int i = 0; i < MEM_SIZE; i++) {
        vm->arena[i] = 0;
    }
}

void dump_arena(const vm_t *vm)
{
    if (vm == NULL) {
        return;
    }
    for (int i = 0; i < MEM_SIZE; i++) {
        printf("%02x", vm->arena[i]);
        if ((i + 1) % 32 == 0) {
            printf("\n");
        } else {
            printf(" ");
        }
    }
}
