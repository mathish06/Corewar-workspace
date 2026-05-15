/*
** EPITECH PROJECT, 2025
** basic_instructions.c
** File description:
** basic_instructions for corewar
*/
#include "../../include/corewar.h"

void exec_live(global_t *global, vm_t *vm, process_t *curr)
{
    int player_nb;
    int idx = (curr->pc + 1) % MEM_SIZE;

    player_nb = (vm->arena[idx] << 24)
        | (vm->arena[(idx + 1) % MEM_SIZE] << 16)
        | (vm->arena[(idx + 2) % MEM_SIZE] << 8)
        | (vm->arena[(idx + 3) % MEM_SIZE]);
    curr->is_alive = 1;
    vm->live_count += 1;
    for (int i = 0; i < global->nbr_champions; i++) {
        if (global->champions[i].prog_number == player_nb) {
            my_putstr("The player ");
            my_put_nbr(global->champions[i].prog_number);
            my_putstr("(");
            my_putstr(global->champions[i].header.prog_name);
            my_putstr(")");
            my_putstr(" is alive.\n");
        }
    }
    curr->pc = (curr->pc + 5) % MEM_SIZE;
}

void exec_zjmp(global_t *global, vm_t *vm, process_t *curr)
{
    short offset;

    (void)global;
    if (curr->carry == 0)
        curr->pc = (curr->pc + 3) % MEM_SIZE;
    if (curr->carry == 1) {
        offset = (vm->arena[(curr->pc + 1) % MEM_SIZE] << 8)
            | (vm->arena[(curr->pc + 2) % MEM_SIZE]);
        curr->pc = (curr->pc + (offset % IDX_MOD)) % MEM_SIZE;
        if (curr->pc < 0)
            curr->pc = curr->pc + MEM_SIZE;
    }
}
