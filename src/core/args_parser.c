/*
** EPITECH PROJECT, 2026
** args_parser.c
** File description:
** Argument parsing utilities for Corewar command-line options
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

int parse_single_arg(char **argv, int *i, global_t *global)
{
    if (my_strcmp("-dump", argv[*i]) == 0)
        return handle_dump(argv, i, global);
    if (my_strcmp("-n", argv[*i]) == 0)
        return handle_n_flag(argv, i, global);
    if (my_strcmp("-a", argv[*i]) == 0)
        return handle_a_flag(argv, i, global);
    return handle_champion(argv[*i], global);
}
