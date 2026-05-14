/*
** EPITECH PROJECT, 2026
** main.c
** File description:
** main
*/

#include "corewar.h"

static void my_putstr_err(const char *str)
{
    for (int i = 0; str && str[i] != '\0'; i++)
        write(2, &str[i], 1);
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        my_putstr_err("Usage: ");
        my_putstr_err(argv[0]);
        my_putstr_err(" [-dump N] [-n N champ.cor] [-a N champ.cor] ...\n");
        return 84;
    }
    return corewar(argc, argv);
}
