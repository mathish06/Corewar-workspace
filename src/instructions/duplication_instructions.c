/*
** EPITECH PROJECT, 2026
** duplication_instructions.c
** File description:
** duplication_instructions for corewar
*/
#include "../../include/corewar.h"

static process_t *duplicate_process(vm_t *vm, process_t *curr)
{
    process_t *clone;

    clone = malloc(sizeof(process_t));
    if (!clone)
        return NULL;
    clone->carry = curr->carry;
    clone->current_opcode = curr->current_opcode;
    clone->cycle_to_wait = 0;
    clone->is_alive = curr->is_alive;
    for (int i = 0; i < REG_NUMBER; i++)
        clone->registers[i] = curr->registers[i];
    clone->next = vm->process_list;
    vm->process_list = clone;
    return clone;
}

void exec_fork(global_t *global, vm_t *vm, process_t *curr)
{
    short offset;
    process_t *clone;

    (void)global;
    offset = (vm->arena[(curr->pc + 1) % MEM_SIZE] << 8)
        | (vm->arena[(curr->pc + 2) % MEM_SIZE]);
    clone = duplicate_process(vm, curr);
    if (!clone)
        return;
    clone->pc = (curr->pc + (offset % IDX_MOD)) % MEM_SIZE;
    if (clone->pc < 0)
        clone->pc = clone->pc + MEM_SIZE;
    curr->pc = (curr->pc + 3) % MEM_SIZE;
}

void exec_lfork(global_t *global, vm_t *vm, process_t *curr)
{
    short offset;
    process_t *clone;

    (void)global;
    offset = (vm->arena[(curr->pc + 1) % MEM_SIZE] << 8)
        | (vm->arena[(curr->pc + 2) % MEM_SIZE]);
    clone = duplicate_process(vm, curr);
    if (!clone)
        return;
    clone->pc = (curr->pc + offset) % MEM_SIZE;
    if (clone->pc < 0)
        clone->pc = clone->pc + MEM_SIZE;
    curr->pc = (curr->pc + 3) % MEM_SIZE;
}
