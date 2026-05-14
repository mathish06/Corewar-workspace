/*
** EPITECH PROJECT, 2026
** coding_byte.c
** File description:
** Decoding the coding byte
*/

#include "../../include/corewar.h"

void decode_coding_byte(uint8_t cb, int types[4])
{
    int raw;
    int i;
    static const int map[4] = {0, T_REG, T_DIR, T_IND};

    if (types == NULL)
        return;
    for (i = 0; i < 4; i++) {
        raw = (cb >> (6 - (i * 2))) & 3;
        types[i] = map[raw];
    }
}
