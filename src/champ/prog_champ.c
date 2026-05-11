/*
** EPITECH PROJECT, 2025
** prog_champ.c
** File description:
** prog_champ for corewar
*/
#include "../../include/corewar.h"

void prep_usernames_and_addresses(global_t *global)
{
    int champ_nbr = 1;
    int gap = MEM_SIZE / global->nbr_champions;

    for (int i = 0; i <= global->nbr_champions; i++) {
        if (global->champions[i].flag_n == -1) {
            global->champions[i].prog_number = champ_nbr;
            champ_nbr++;
        } else {
            global->champions[i].prog_number = global->champions[i].flag_n;
        }
    }
    for (int i = 0; i <= global->nbr_champions; i++) {
        if (global->champions[i].flag_a == -1)
            global->champions[i].load_address = i * gap;
        else {
            global->champions[i].load_address = global->champions[i].flag_a
                % MEM_SIZE;
        }
    }
}
