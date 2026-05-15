/*
** EPITECH PROJECT, 2026
** instructions_calc.c
** File description:
** corewar
*/

#include "../../include/corewar.h"

static int get_val_reg(vm_t *vm, process_t *proc, int *pos)
{
    int val = 0;
    int reg = vm->arena[*pos % MEM_SIZE];

    if (reg >= 1 && reg <= REG_NUMBER)
        val = proc->registers[reg - 1];
    *pos += 1;
    return val;
}

static int get_val_dir(vm_t *vm, int *pos)
{
    int val = 0;

    for (int i = 0; i < 4; i++)
        val = (val << 8) | vm->arena[(*pos + i) % MEM_SIZE];
    *pos += 4;
    return val;
}

static int get_val_ind(vm_t *vm, process_t *proc, int *pos)
{
    short addr = 0;
    int target = 0;
    int val = 0;

    for (int i = 0; i < 2; i++)
        addr = (addr << 8) | vm->arena[(*pos + i) % MEM_SIZE];
    if (proc->current_opcode == 13)
        target = (proc->pc + addr) % MEM_SIZE;
    else
        target = (proc->pc + (addr % IDX_MOD)) % MEM_SIZE;
    if (target < 0)
        target += MEM_SIZE;
    for (int i = 0; i < 4; i++)
        val = (val << 8) | vm->arena[(target + i) % MEM_SIZE];
    *pos += 2;
    return val;
}

int get_val(vm_t *vm, process_t *proc, int type, int *pos)
{
    if (type == T_REG)
        return get_val_reg(vm, proc, pos);
    if (type == T_DIR)
        return get_val_dir(vm, pos);
    if (type == T_IND)
        return get_val_ind(vm, proc, pos);
    return 0;
}

void op_sub(global_t *global, vm_t *vm, process_t *proc)
{
    int r1 = vm->arena[(proc->pc + 2) % MEM_SIZE] - 1;
    int r2 = vm->arena[(proc->pc + 3) % MEM_SIZE] - 1;
    int r3 = vm->arena[(proc->pc + 4) % MEM_SIZE] - 1;

    (void)global;
    if (r1 < 0 || r1 >= REG_NUMBER || r2 < 0 || r2 >= REG_NUMBER)
        return;
    if (r3 < 0 || r3 >= REG_NUMBER)
        return;
    proc->registers[r3] = proc->registers[r1] - proc->registers[r2];
    proc->carry = (proc->registers[r3] == 0);
    proc->pc = (proc->pc + 5) % MEM_SIZE;
}

void op_and(global_t *global, vm_t *vm, process_t *proc)
{
    int types[4];
    int pos = (proc->pc + 2) % MEM_SIZE;
    int v1 = 0;
    int v2 = 0;
    int r3 = 0;

    (void)global;
    decode_coding_byte(vm->arena[(proc->pc + 1) % MEM_SIZE], types);
    v1 = get_val(vm, proc, types[0], &pos);
    v2 = get_val(vm, proc, types[1], &pos);
    r3 = vm->arena[pos % MEM_SIZE] - 1;
    if (r3 >= 0 && r3 < REG_NUMBER) {
        proc->registers[r3] = v1 & v2;
        proc->carry = (proc->registers[r3] == 0);
    }
    proc->pc = (pos + 1) % MEM_SIZE;
}

void op_or(global_t *global, vm_t *vm, process_t *proc)
{
    int types[4];
    int pos = (proc->pc + 2) % MEM_SIZE;
    int v1 = 0;
    int v2 = 0;
    int r3 = 0;

    (void)global;
    decode_coding_byte(vm->arena[(proc->pc + 1) % MEM_SIZE], types);
    v1 = get_val(vm, proc, types[0], &pos);
    v2 = get_val(vm, proc, types[1], &pos);
    r3 = vm->arena[pos % MEM_SIZE] - 1;
    if (r3 >= 0 && r3 < REG_NUMBER) {
        proc->registers[r3] = v1 | v2;
        proc->carry = (proc->registers[r3] == 0);
    }
    proc->pc = (pos + 1) % MEM_SIZE;
}

void op_xor(global_t *global, vm_t *vm, process_t *proc)
{
    int types[4];
    int pos = (proc->pc + 2) % MEM_SIZE;
    int v1 = 0;
    int v2 = 0;
    int r3 = 0;

    (void)global;
    decode_coding_byte(vm->arena[(proc->pc + 1) % MEM_SIZE], types);
    v1 = get_val(vm, proc, types[0], &pos);
    v2 = get_val(vm, proc, types[1], &pos);
    r3 = vm->arena[pos % MEM_SIZE] - 1;
    if (r3 >= 0 && r3 < REG_NUMBER) {
        proc->registers[r3] = v1 ^ v2;
        proc->carry = (proc->registers[r3] == 0);
    }
    proc->pc = (pos + 1) % MEM_SIZE;
}
