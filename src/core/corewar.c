/*
** EPITECH PROJECT, 2026
** corewar.c
** File description:
** Main entry and VM orchestration for the Corewar program
*/

#include "../../include/corewar.h"

static void init_structs(global_t *global, vm_t *vm)
{
    global->dump = -1;
    global->nbr_champions = 0;
    global->temp_n = -1;
    global->temp_a = -1;
    vm->process_list = NULL;
    vm->total_cycles = 0;
    vm->current_cycle = 0;
    vm->live_count = 0;
    vm->cycle_to_die = CYCLE_TO_DIE;
    global->last_live_id = 0;
}

static int setup_vm(global_t *global, vm_t *vm)
{
    init_vm(vm);
    for (int i = 0; i < global->nbr_champions; i++) {
        if (load_champion(&global->champions[i],
                global->champions[i].filepath) == 84)
            return 84;
    }
    prep_usernames_and_addresses(global);
    global->last_live_id =
        global->champions[global->nbr_champions - 1].prog_number;
    entry_into_arena(global, vm);
    create_initial_processes(global, vm);
    return 0;
}

static int display_help(void)
{
    my_putstr("USAGE\n");
    my_putstr("./corewar [-dump nbr_cycle] [[-n prog_number] [-a load_address] "
        "prog_name] ...\n");
    my_putstr("DESCRIPTION\n");
    my_putstr("-dump nbr_cycle dumps the state of the virtual machine after "
        "the nbr_cycle execution\n");
    my_putstr("-n prog_number sets the next program's number. By default, "
        "the first free number in the parameter order\n");
    my_putstr("-a load_address sets the next program's loading address. "
        "When no address is specified, optimize the addresses so that the "
        "processes are as far away from each other as possible. "
        "The addresses are MEM_SIZE modulo.\n");
    return 0;
}

static void cleanup_vm(global_t *global, vm_t *vm)
{
    process_t *curr = vm->process_list;
    process_t *tmp;

    while (curr != NULL) {
        tmp = curr;
        curr = curr->next;
        free(tmp);
    }
    vm->process_list = NULL;
    for (int i = 0; i < global->nbr_champions; i++)
        free_champion(&global->champions[i]);
}

int corewar(int argc, char **argv)
{
    global_t global;
    vm_t vm;

    if (argc == 2 && my_strcmp(argv[1], "-h") == 0)
        return display_help();
    init_structs(&global, &vm);
    for (int i = 1; i < argc; i++) {
        if (parse_single_arg(argv, &i, &global) == 84)
            return 84;
    }
    if (global.nbr_champions == 0)
        return 84;
    if (setup_vm(&global, &vm) == 84)
        return 84;
    game_loop(&global, &vm);
    display_winner(&global);
    cleanup_vm(&global, &vm);
    return 0;
}
