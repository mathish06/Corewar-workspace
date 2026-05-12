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

    #define MEM_SIZE (6 * 1024)

    #include "corwar_struct.h"

int load_champion(champion_t *champ, const char *filepath);
void free_champion(champion_t *champ);
int swap_int32(int val);
void prep_usernames_and_addresses(global_t *global);
void entry_into_arena(global_t *global, vm_t *vm);
void create_initial_processes(global_t *global, vm_t *vm);

void init_vm(vm_t *vm);
void dump_arena(const vm_t *vm);

int my_str_isnum(char *str);

int corewar(int argc, char **argv);
int game_loop(global_t *global, vm_t *vm);


int my_strcmp(char const *s1, char const *s2);
int my_getnbr(char const *str);

#endif
