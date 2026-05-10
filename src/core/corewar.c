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

static int handle_n_flag(char **argv, int *i, int *temp_n)
{
    int nbr;

    if (argv[*i + 1] == NULL)
        return 84;
    if (my_str_isnum(argv[*i + 1]) == 0)
        return 84;
    nbr = my_getnbr(argv[*i + 1]);
    *temp_n = nbr;
    (*i)++;
    return 0;
}

int corewar(int argc, char **argv)
{
    global_t global;
    int temp_n = -1;
    int temp_a = -1;

    global.dump = -1;
    global.nbr_champions = 0;
    for (int i = 1; i < argc; i++) {
        
    }
}
