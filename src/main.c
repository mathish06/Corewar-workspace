/*
** EPITECH PROJECT, 2026
** main.c
** File description:
** Main entry
*/

#include "../include/corewar.h"

int main(int argc, char **argv)
{
    champion_t champ;
    int ret;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <champion.cor>\n", argv[0]);
        return 84;
    }
    ret = load_champion(&champ, argv[1]);
    if (ret == 0) {
        free_champion(&champ);
    }
    return ret;
}
