#include <criterion/criterion.h>
#include "../include/corewar.h"

Test(endianness, swap_int32) {
    int val = 0x12345678;
    int expected = 0x78563412;
    cr_assert_eq(swap_int32(val), expected);
}

Test(arena, init_vm_zeros_memory) {
    vm_t vm;

    for (int i = 0; i < MEM_SIZE; i++) {
        vm.arena[i] = 42;
    }
    init_vm(&vm);
    for (int i = 0; i < MEM_SIZE; i++) {
        cr_assert_eq(vm.arena[i], 0);
    }
}
