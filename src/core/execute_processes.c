/*
** EPITECH PROJECT, 2026
** execute_processes.c
** File description:
** execute_processes for corewar
*/
#include "../../include/corewar.h"

static void fetch_instruction(vm_t *vm, process_t *curr)
{
    if (curr->cycle_to_wait == 0) {
        curr->current_opcode = vm->arena[curr->pc];
        if (curr->current_opcode >= 1 && curr->current_opcode <= 16) {
            curr->cycle_to_wait = op_tab[curr->current_opcode].nbr_cycles;
        }
    }
}

static void cooldown(process_t *curr)
{
    if (curr->cycle_to_wait > 0)
        curr->cycle_to_wait--;
}

static void execution(process_t *curr)
{
    if (curr->cycle_to_wait == 0)
        curr->pc = (curr->pc + 1) % MEM_SIZE;
}
