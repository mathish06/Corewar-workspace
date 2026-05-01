/*
** EPITECH PROJECT, 2026
** corewar.h
** File description:
** the .h for corewar
*/

#ifndef COREWAR_H_
    #define COREWAR_H_

    #include "./op.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <stdint.h>

int load_champion_header(const char *filepath);
int swap_int32(int val);

#endif
