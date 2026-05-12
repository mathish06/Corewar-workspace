/*
** EPITECH PROJECT, 2026
** arena.c
** File description:
** Arena initialization and dump
*/

#include "../../include/corewar.h"

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
    const char *hex = "0123456789abcdef";
    char buf[2];
    char space = ' ';
    char nl = '\n';

    if (vm == NULL) {
        return;
    }
    for (int i = 0; i < MEM_SIZE; i++) {
        buf[0] = hex[vm->arena[i] >> 4];
        buf[1] = hex[vm->arena[i] & 0x0F];
        write(1, buf, 2);
        if ((i + 1) % 32 == 0) {
            write(1, &nl, 1);
        } else {
            write(1, &space, 1);
        }
    }
}
