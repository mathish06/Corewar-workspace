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

    if (types == NULL)
        return;
    for (int i = 0; i < 4; i++) {
        raw = (cb >> (6 - (i * 2))) & 3;
        switch (raw) {
            case 1:
                types[i] = T_REG;
                break;
            case 2:
                types[i] = T_DIR;
                break;
            case 3:
                types[i] = T_IND;
                break;
            default:
                types[i] = 0;
                break;
        }
    }
}
