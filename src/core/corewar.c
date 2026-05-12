/*
** EPITECH PROJECT, 2025
** corewar.c
** File description:
** corewar.c to handle args
*/
#include "../../include/corewar.h"

static int handle_dump(char **argv, int *i, global_t *global)
{
    int nbr;

    if (argv[*i + 1] == NULL)
        return 84;
    if (my_str_isnum(argv[*i + 1]) == 0)
        return 84;
    nbr = my_getnbr(argv[*i + 1]);
    global->dump = nbr;
    (*i)++;
    return 0;
}

static int handle_n_flag(char **argv, int *i, global_t *global)
{
    int nbr;

    if (argv[*i + 1] == NULL)
        return 84;
    if (my_str_isnum(argv[*i + 1]) == 0)
        return 84;
    nbr = my_getnbr(argv[*i + 1]);
    global->temp_n = nbr;
    (*i)++;
    return 0;
}

static int handle_a_flag(char **argv, int *i, global_t *global)
{
    int nbr;

    if (argv[*i + 1] == NULL)
        return 84;
    if (my_str_isnum(argv[*i + 1]) == 0)
        return 84;
    nbr = my_getnbr(argv[*i + 1]);
    global->temp_a = nbr;
    (*i)++;
    return 0;
}

static int handle_champion(char *filepath, global_t *global)
{
    if (global->nbr_champions >= MAX_ARGS_NUMBER)
        return 84;
    global->champions[global->nbr_champions].filepath = filepath;
    global->champions[global->nbr_champions].flag_n = global->temp_n;
    global->champions[global->nbr_champions].flag_a = global->temp_a;
    global->temp_n = -1;
    global->temp_a = -1;
    global->nbr_champions++;
    return 0;
}

static int parse_single_arg(char **argv, int *i, global_t *global)
{
    if (my_strcmp("-dump", argv[*i]) == 0)
        return handle_dump(argv, i, global);
    if (my_strcmp("-n", argv[*i]) == 0)
        return handle_n_flag(argv, i, global);
    if (my_strcmp("-a", argv[*i]) == 0)
        return handle_a_flag(argv, i, global);
    return handle_champion(argv[*i], global);
}

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
    entry_into_arena(global, vm);
    create_initial_processes(global, vm);
    return 0;
}

int corewar(int argc, char **argv)
{
    global_t global;
    vm_t vm;

    init_structs(&global, &vm);
    for (int i = 1; i < argc; i++) {
        if (parse_single_arg(argv, &i, &global) == 84)
            return 84;
    }
    if (global.nbr_champions == 0)
        return 84;
    if (setup_vm(&global, &vm) == 84)
        return 84;
    dump_arena(&vm);
    return 0;
}
