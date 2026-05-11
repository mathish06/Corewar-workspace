#include <criterion/criterion.h>
#include <string.h>
#include <criterion/redirect.h>
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

Test(tools, my_str_isnum_valid) {
    cr_assert_eq(my_str_isnum("42"), 1);
}

Test(tools, my_str_isnum_invalid) {
    cr_assert_eq(my_str_isnum("42a"), 0);
}

Test(tools, my_strcmp_equal) {
    cr_assert_eq(my_strcmp("-dump", "-dump"), 0);
}

Test(tools, my_strcmp_diff) {
    cr_assert_neq(my_strcmp("-dump", "-n"), 0);
}

Test(corewar_parsing, missing_champion) {
    char *argv[] = {"./corewar", "-dump", "10", NULL};
    cr_assert_eq(corewar(3, argv), 84);
}

Test(corewar_parsing, invalid_dump_value) {
    char *argv[] = {"./corewar", "-dump", "abc", "champ.cor", NULL};
    cr_assert_eq(corewar(4, argv), 84);
}

Test(corewar_parsing, missing_flag_value) {
    char *argv[] = {"./corewar", "-n", NULL};
    cr_assert_eq(corewar(2, argv), 84);
}

Test(corewar_parsing, invalid_n_flag) {
    char *argv[] = {"./corewar", "-n", "test", "champ.cor", NULL};
    cr_assert_eq(corewar(4, argv), 84);
}

Test(corewar_parsing, valid_arguments) {
    cr_redirect_stdout();
    cr_redirect_stderr();
    header_t h;
    memset(&h, 0, sizeof(header_t));
    h.magic = swap_int32(COREWAR_EXEC_MAGIC);
    h.prog_size = swap_int32(4);
    uint8_t payload[4] = {1, 2, 3, 4};
    FILE *f = fopen("champ.cor", "w");
    fwrite(&h, sizeof(header_t), 1, f);
    fwrite(payload, 1, 4, f);
    fclose(f);
    char *argv[] = {"./corewar", "-dump", "10", "-n", "1", "-a", "512", "champ.cor", NULL};
    cr_assert_eq(corewar(8, argv), 0);
    remove("champ.cor");
}

Test(corewar_parsing, too_many_champions) {
    char *argv[] = {"./corewar", "1.cor", "2.cor", "3.cor", "4.cor", "5.cor", NULL};
    cr_assert_eq(corewar(6, argv), 84);
}

Test(champion, load_non_existent) {
    champion_t champ;
    cr_assert_eq(load_champion(&champ, "non_existent_file_xyz.cor"), 84);
}

Test(champion, free_champion_null) {
    champion_t champ;
    champ.code = NULL;
    free_champion(&champ);
    free_champion(NULL);
}

Test(champion, load_empty_file) {
    champion_t champ;
    FILE *f = fopen("empty.cor", "w");
    fclose(f);
    cr_assert_eq(load_champion(&champ, "empty.cor"), 84);
    remove("empty.cor");
}

Test(champion, load_bad_magic) {
    champion_t champ;
    header_t h;
    memset(&h, 0, sizeof(header_t));
    h.magic = swap_int32(0x123456);
    FILE *f = fopen("bad_magic.cor", "w");
    fwrite(&h, sizeof(header_t), 1, f);
    fclose(f);
    cr_assert_eq(load_champion(&champ, "bad_magic.cor"), 84);
    remove("bad_magic.cor");
}

Test(champion, load_bad_size) {
    champion_t champ;
    header_t h;
    memset(&h, 0, sizeof(header_t));
    h.magic = swap_int32(COREWAR_EXEC_MAGIC);
    h.prog_size = swap_int32(9999999);
    FILE *f = fopen("bad_size.cor", "w");
    fwrite(&h, sizeof(header_t), 1, f);
    fclose(f);
    cr_assert_eq(load_champion(&champ, "bad_size.cor"), 84);
    remove("bad_size.cor");
}

Test(champion, load_truncated_payload) {
    champion_t champ;
    header_t h;
    memset(&h, 0, sizeof(header_t));
    h.magic = swap_int32(COREWAR_EXEC_MAGIC);
    h.prog_size = swap_int32(10);
    FILE *f = fopen("truncated.cor", "w");
    fwrite(&h, sizeof(header_t), 1, f);
    fclose(f);
    cr_assert_eq(load_champion(&champ, "truncated.cor"), 84);
    remove("truncated.cor");
}

Test(champion, load_trailing_bytes) {
    champion_t champ;
    header_t h;
    uint8_t payload[4] = {1, 2, 3, 4};
    uint8_t trailing = 0xFF;
    memset(&h, 0, sizeof(header_t));
    h.magic = swap_int32(COREWAR_EXEC_MAGIC);
    h.prog_size = swap_int32(4);
    FILE *f = fopen("trailing.cor", "w");
    fwrite(&h, sizeof(header_t), 1, f);
    fwrite(payload, 1, 4, f);
    fwrite(&trailing, 1, 1, f);
    fclose(f);
    cr_assert_eq(load_champion(&champ, "trailing.cor"), 84);
    remove("trailing.cor");
}

Test(champion, load_valid_champion) {
    champion_t champ;
    header_t h;
    uint8_t payload[4] = {1, 2, 3, 4};
    
    cr_redirect_stdout();
    memset(&h, 0, sizeof(header_t));
    h.magic = swap_int32(COREWAR_EXEC_MAGIC);
    h.prog_size = swap_int32(4);
    strcpy(h.prog_name, "ValidChamp");
    strcpy(h.comment, "This is valid");
    FILE *f = fopen("valid.cor", "w");
    fwrite(&h, sizeof(header_t), 1, f);
    fwrite(payload, 1, 4, f);
    fclose(f);

    cr_assert_eq(load_champion(&champ, "valid.cor"), 0);
    cr_assert_eq(champ.header.prog_size, 4);
    cr_assert_neq(champ.code, NULL);
    
    free_champion(&champ);
    remove("valid.cor");
}

Test(champion, load_zero_size_champion) {
    champion_t champ;
    header_t h;
    
    cr_redirect_stdout();
    memset(&h, 0, sizeof(header_t));
    h.magic = swap_int32(COREWAR_EXEC_MAGIC);
    h.prog_size = swap_int32(0);
    FILE *f = fopen("zero.cor", "w");
    fwrite(&h, sizeof(header_t), 1, f);
    fclose(f);

    cr_assert_eq(load_champion(&champ, "zero.cor"), 0);
    cr_assert_eq(champ.code, NULL);
    
    free_champion(&champ);
    remove("zero.cor");
}

Test(prog_champ, ids_and_addresses) {
    global_t global;
    global.nbr_champions = 2;
    global.champions[0].flag_n = -1;
    global.champions[0].flag_a = -1;
    global.champions[1].flag_n = 5;
    global.champions[1].flag_a = 100;
    prep_usernames_and_addresses(&global);
    cr_assert_eq(global.champions[1].prog_number, 5);
    cr_assert_eq(global.champions[0].prog_number, 1);
    cr_assert_eq(global.champions[0].load_address, 0);
    cr_assert_eq(global.champions[1].load_address, 100);
}

Test(prog_champ, arena_and_processes) {
    global_t global;
    vm_t vm;
    vm.process_list = NULL;
    for(int i = 0; i < MEM_SIZE; i++) 
        vm.arena[i] = 0;
    global.nbr_champions = 1;
    global.champions[0].prog_number = 42;
    global.champions[0].header.prog_size = 4;
    global.champions[0].load_address = MEM_SIZE - 2;
    uint8_t code[4] = {0x0A, 0x0B, 0x0C, 0x0D};
    global.champions[0].code = code;
    entry_into_arena(&global, &vm);
    cr_assert_eq(vm.arena[MEM_SIZE - 2], 0x0A);
    cr_assert_eq(vm.arena[MEM_SIZE - 1], 0x0B);
    cr_assert_eq(vm.arena[0], 0x0C);
    cr_assert_eq(vm.arena[1], 0x0D);
    create_initial_processes(&global, &vm);
    cr_assert_neq(vm.process_list, NULL);
    cr_assert_eq(vm.process_list->pc, MEM_SIZE - 2);
    cr_assert_eq(vm.process_list->registers[0], 42);
    cr_assert_eq(vm.process_list->carry, 0);
    free(vm.process_list); 
}
