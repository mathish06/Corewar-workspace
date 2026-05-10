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
    #include <./corwar_struct.h>

typedef struct champion_s {
    header_t header;
    uint8_t *code;
    int load_address;
    int prog_number;
} champion_t;

int load_champion(champion_t *champ, const char *filepath);
void free_champion(champion_t *champ);
int swap_int32(int val);

int my_str_isnum(char *str);



int my_strcmp(char const *s1, char const *s2);

#endif
