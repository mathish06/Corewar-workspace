/*
** EPITECH PROJECT, 2025
** basic_instructions.c
** File description:
** basic_instructions for corewar
*/
#include "../../include/corewar.h"

static void print_live_message(global_t *global, int player_nb)
{
    for (int i = 0; i < global->nbr_champions; i++) {
        if (global->champions[i].prog_number == player_nb) {
            my_putstr("The player ");
            my_put_nbr(global->champions[i].prog_number);
            my_putstr("(");
            my_putstr(global->champions[i].header.prog_name);
            my_putstr(") is alive.\n");
            global->last_live_id = player_nb;
        }
    }
}

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
    print_live_message(global, player_nb);
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

void exec_print(global_t *global, vm_t *vm, process_t *curr)
{
    int reg_nb;
    int reg_val;

    (void)global;
    reg_nb = vm->arena[(curr->pc + 2) % MEM_SIZE];
    if (reg_nb >= 1 && reg_nb <= REG_NUMBER) {
        reg_val = curr->registers[reg_nb - 1];
        reg_val = reg_val % 256;
        my_putchar(reg_val);
    }
    curr->pc = (curr->pc + 3) % MEM_SIZE;
}
