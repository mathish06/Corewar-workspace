/*
** EPITECH PROJECT, 2026
** endian.c
** File description:
** Endianness swap utility for corewar
*/

#include "../../include/corewar.h"

int swap_int32(int val)
{
    uint32_t v = (uint32_t)val;
    uint32_t res = (v >> 24)
        | ((v << 8) & 0xff0000)
        | ((v >> 8) & 0xff00)
        | (v << 24);

    return (int)res;
}
