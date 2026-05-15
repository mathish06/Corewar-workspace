/*
** EPITECH PROJECT, 2026
** game_loop.c
** File description:
** game_loop for corewar
*/
#include "../../include/corewar.h"

static void unlink_process(vm_t *vm, process_t *prev, process_t *curr)
{
    if (prev == NULL)
        vm->process_list = curr->next;
    else
        prev->next = curr->next;
}

static void delete_node(vm_t *vm)
{
    process_t *prev = NULL;
    process_t *curr = vm->process_list;
    process_t *tmp;

    while (curr != NULL) {
        if (curr->is_alive == 0) {
            tmp = curr;
            unlink_process(vm, prev, curr);
            curr = curr->next;
            free(tmp);
        } else {
            curr->is_alive = 0;
            prev = curr;
            curr = curr->next;
        }
    }
}

static void handle_verification(vm_t *vm)
{
    if (vm->current_cycle == vm->cycle_to_die) {
        delete_node(vm);
        if (vm->live_count >= NBR_LIVE)
            vm->cycle_to_die -= CYCLE_DELTA;
        vm->current_cycle = 0;
        vm->live_count = 0;
    }
}

void display_winner(global_t *global)
{
    for (int i = 0; i < global->nbr_champions; i++) {
        if (global->champions[i].prog_number == global->last_live_id) {
            my_putstr("The player ");
            my_put_nbr(global->champions[i].prog_number);
            my_putstr("(");
            my_putstr(global->champions[i].header.prog_name);
            my_putstr(")");
            my_putstr(" has won.\n");
        }
    }
}

int game_loop(global_t *global, vm_t *vm)
{
    while (vm->process_list != NULL && vm->cycle_to_die > 0) {
        if (vm->total_cycles == global->dump) {
            dump_arena(vm);
            return 0;
        }
        execute_processes(global, vm);
        vm->current_cycle++;
        vm->total_cycles++;
        handle_verification(vm);
    }
    return 0;
}
