/*
** EPITECH PROJECT, 2025
** prog_champ.c
** File description:
** prog_champ for corewar
*/
#include "../../include/corewar.h"

static int is_id_taken(global_t *global, int id)
{
    for (int i = 0; i < global->nbr_champions; i++) {
        if (global->champions[i].prog_number == id)
            return 1;
    }
    return 0;
}

static void assign_missing_ids(global_t *global, int i)
{
    int candidate_id = 1;

    if (global->champions[i].prog_number == -1) {
        while (is_id_taken(global, candidate_id))
            candidate_id++;
        global->champions[i].prog_number = candidate_id;
    }
}

static void assign_load_addresses(global_t *global)
{
    int gap = MEM_SIZE / global->nbr_champions;

    for (int i = 0; i < global->nbr_champions; i++) {
        if (global->champions[i].flag_a == -1)
            global->champions[i].load_address = i * gap;
        else {
            global->champions[i].load_address = global->champions[i].flag_a
                % MEM_SIZE;
        }
    }
}

void prep_usernames_and_addresses(global_t *global)
{
    for (int i = 0; i < global->nbr_champions; i++) {
        if (global->champions[i].flag_n != -1)
            global->champions[i].prog_number = global->champions[i].flag_n;
        else
            global->champions[i].prog_number = -1;
    }
    for (int i = 0; i < global->nbr_champions; i++)
        assign_missing_ids(global, i);
    assign_load_addresses(global);
}

void entry_into_arena(global_t *global, vm_t *vm)
{
    int start;
    int dest;

    for (int i = 0; i < global->nbr_champions; i++) {
        start = global->champions[i].load_address;
        for (int y = 0; y < global->champions[i].header.prog_size; y++) {
            dest = (start + y) % MEM_SIZE;
            vm->arena[dest] = global->champions[i].code[y];
        }
    }
}

void create_initial_processes(global_t *global, vm_t *vm)
{
    process_t *new_process;

    for (int i = 0; i < global->nbr_champions; i++) {
        new_process = malloc(sizeof(process_t));
        if (!new_process)
            return;
        new_process->pc = global->champions[i].load_address;
        new_process->carry = 0;
        for (int y = 0; y < REG_NUMBER; y++)
            new_process->registers[y] = 0;
        new_process->registers[0] = global->champions[i].prog_number;
        new_process->is_alive = 0;
        new_process->cycle_to_wait = 0;
        new_process->current_opcode = 0;
        new_process->next = vm->process_list;
        vm->process_list = new_process;
    }
}
