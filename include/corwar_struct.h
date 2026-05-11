/*
** EPITECH PROJECT, 2026
** corewar_struct.h
** File description:
** the .h for corewar
*/

#ifndef COREWAR_STRUCT_H_
    #define COREWAR_STRUCT_H_

typedef struct champion_s {
    header_t header;
    uint8_t *code;
    int load_address;
    int prog_number;
    int flag_n;
    int flag_a;
    char *filepath;
} champion_t;

typedef struct global_s {
    int dump;
    champion_t champions[MAX_ARGS_NUMBER];
    int nbr_champions;
    int temp_n;
    int temp_a;
} global_t;

typedef struct process_s {
    int pc;                     
    int carry;                  
    int registers[REG_NUMBER];  
    struct process_s *next;     
} process_t;

typedef struct vm_s {
    uint8_t arena[MEM_SIZE];
    process_t *process_list;
} vm_t;

#endif
