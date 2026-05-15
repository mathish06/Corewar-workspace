/*
** EPITECH PROJECT, 2026
** instructions_mem.c
** File description:
** corewar
*/

#include "../../include/corewar.h"

void op_ld(global_t *global, vm_t *vm, process_t *proc)
{
    int types[4];
    int pos = (proc->pc + 2) % MEM_SIZE;
    int val;
    int reg;

    (void)global;
    decode_coding_byte(vm->arena[(proc->pc + 1) % MEM_SIZE], types);
    val = get_val(vm, proc, types[0], &pos);
    reg = vm->arena[pos % MEM_SIZE] - 1;
    if (reg >= 0 && reg < REG_NUMBER) {
        proc->registers[reg] = val;
        proc->carry = (val == 0);
    }
    proc->pc = (pos + 1) % MEM_SIZE;
}

static void write_st_ind(vm_t *vm, process_t *proc, int val, int *pos)
{
    short addr = 0;
    int target;

    for (int i = 0; i < 2; i++)
        addr = (addr << 8) | vm->arena[(*pos + i) % MEM_SIZE];
    *pos += 2;
    target = (proc->pc + (addr % IDX_MOD)) % MEM_SIZE;
    if (target < 0)
        target += MEM_SIZE;
    for (int i = 0; i < 4; i++)
        vm->arena[(target + i) % MEM_SIZE] = (val >> (8 * (3 - i))) & 0xFF;
}

void op_st(global_t *global, vm_t *vm, process_t *proc)
{
    int types[4];
    int pos = (proc->pc + 2) % MEM_SIZE;
    int val;
    int reg;

    (void)global;
    decode_coding_byte(vm->arena[(proc->pc + 1) % MEM_SIZE], types);
    val = get_val(vm, proc, types[0], &pos);
    if (types[1] == T_REG) {
        reg = vm->arena[pos % MEM_SIZE] - 1;
        if (reg >= 0 && reg < REG_NUMBER)
            proc->registers[reg] = val;
        pos += 1;
    } else if (types[1] == T_IND) {
        write_st_ind(vm, proc, val, &pos);
    }
    proc->pc = pos % MEM_SIZE;
}

void op_lld(global_t *global, vm_t *vm, process_t *proc)
{
    int types[4];
    int pos = (proc->pc + 2) % MEM_SIZE;
    int val;
    int reg;

    (void)global;
    decode_coding_byte(vm->arena[(proc->pc + 1) % MEM_SIZE], types);
    val = get_val(vm, proc, types[0], &pos);
    reg = vm->arena[pos % MEM_SIZE] - 1;
    if (reg >= 0 && reg < REG_NUMBER) {
        proc->registers[reg] = val;
        proc->carry = (val == 0);
    }
    proc->pc = (pos + 1) % MEM_SIZE;
}
