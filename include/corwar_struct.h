/*
** EPITECH PROJECT, 2026
** corewar_struct.h
** File description:
** the .h for corewar
*/

#ifndef COREWAR_STRUCT_H_
    #define COREWAR_STRUCT_H_

typedef struct global_s {
    int dump;
    struct champ_s *champ_list;
} global_t;

typedef struct champ_s {
    int flag_n;
    int flag_a;
    struct champ_s *next;
} champ_t;

#endif
