/*
** EPITECH PROJECT, 2025
** tools.c
** File description:
** tools for corewar
*/
#include "../../include/corewar.h"

int my_str_isnum(char *str)
{
    int i = 0;
    int is_num;
    while (str[i] != '\0') {
        is_num = (str[i] >= '0' && str[i] <= '9');
        if (!is_num)
            return 0;
        i++;
    }
    return 1;
}

