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

static void execution(global_t *global, vm_t *vm, process_t *curr)
{
    static void (*actions[17])(global_t *, vm_t *, process_t *) = {
        NULL, op_live, NULL, NULL, op_add, op_sub, op_and, op_or, op_xor,
        op_zjmp, NULL, NULL, NULL, NULL, NULL, NULL, op_print
    };

    if (curr->cycle_to_wait == 0) {
        if (curr->current_opcode >= 1 && curr->current_opcode <= 16 &&
            actions[curr->current_opcode] != NULL) {
            actions[curr->current_opcode](global, vm, curr);
            curr->current_opcode = 0;
        } else {
            curr->pc = (curr->pc + 1) % MEM_SIZE;
        }
    }
}

void execute_processes(global_t *global, vm_t *vm)
{
    process_t *curr = vm->process_list;

    for (; curr != NULL; curr = curr->next) {
        fetch_instruction(vm, curr);
        cooldown(curr);
        execution(global, vm, curr);
    }
}
