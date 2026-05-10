#include <criterion/criterion.h>
#include "../include/corewar.h"

Test(endianness, swap_int32) {
    int val = 0x12345678;
    int expected = 0x78563412;
    cr_assert_eq(swap_int32(val), expected);
}
