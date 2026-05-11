/*
** EPITECH PROJECT, 2026
** main.c
** File description:
** Main entry
*/

#include "corewar.h"

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [-dump N] [-n N champ.cor]", argv[0]);
        fprintf(stderr, " [-a N champ.cor] ...\n");
        return 84;
    }
    return corewar(argc, argv);
}
