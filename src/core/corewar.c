/*
** EPITECH PROJECT, 2025
** corewar.c
** File description:
** corewar.c to handle args
*/
#include "../../include/corewar.h"

int corewar(int argc, char **argv)
{
    global_t global;
    int count = 0;

    global.dump = -1;
    global.champ_list->flag_a = -1;
    global.champ_list->flag_n = -1;
    for (; argv != NULL; count++) {
        if (my_strcmp("-dump", argc[count]) == 0 && argc[count++] != NULL) {
            if (my_str_isnum(argc[count++]) == 1) {
                
            }
        }
    }
}
