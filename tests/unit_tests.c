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

Test(coding_byte, decode_standard_byte) {
    int types[4];
    decode_coding_byte(0x68, types);
    cr_assert_eq(types[0], T_REG);
    cr_assert_eq(types[1], T_DIR);
    cr_assert_eq(types[2], T_DIR);
    cr_assert_eq(types[3], 0);
}

Test(coding_byte, decode_indirect_byte) {
    int types[4];
    decode_coding_byte(0xF0, types);
    cr_assert_eq(types[0], T_IND);
    cr_assert_eq(types[1], T_IND);
    cr_assert_eq(types[2], 0);
    cr_assert_eq(types[3], 0);
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

Test(game_loop, dump_stops_execution)
{
    global_t global;
    vm_t vm;
    process_t proc;

    global.dump = 5;
    vm.process_list = &proc;
    vm.cycle_to_die = 100;
    vm.total_cycles = 0;
    vm.current_cycle = 0;
    cr_redirect_stdout();
    cr_assert_eq(game_loop(&global, &vm), 0);
    cr_assert_eq(vm.total_cycles, 5);
}

Test(game_loop, process_dies_when_not_alive)
{
    global_t global;
    vm_t vm;
    process_t *proc = malloc(sizeof(process_t));

    global.dump = -1;
    proc->is_alive = 0;
    proc->next = NULL;
    vm.process_list = proc;
    vm.cycle_to_die = 10;
    vm.total_cycles = 0;
    vm.current_cycle = 0;
    vm.live_count = 0;
    cr_redirect_stdout();
    game_loop(&global, &vm);
    cr_assert_null(vm.process_list);
    cr_assert_eq(vm.total_cycles, 10);
}

Test(game_loop, process_survives_then_dies)
{
    global_t global;
    vm_t vm;
    process_t *proc = malloc(sizeof(process_t));

    global.dump = -1;
    proc->is_alive = 1;
    proc->next = NULL;
    vm.process_list = proc;
    vm.cycle_to_die = 15;
    vm.total_cycles = 0;
    vm.current_cycle = 0;
    vm.live_count = 0;
    cr_redirect_stdout();
    game_loop(&global, &vm);
    cr_assert_null(vm.process_list);
    cr_assert_eq(vm.total_cycles, 30);
}

Test(game_loop, difficulty_increases)
{
    global_t global;
    vm_t vm;
    process_t *proc = malloc(sizeof(process_t));

    global.dump = 20;
    proc->is_alive = 1;
    proc->next = NULL;
    vm.process_list = proc;
    vm.cycle_to_die = 20;
    vm.total_cycles = 0;
    vm.current_cycle = 0;
    vm.live_count = NBR_LIVE;
    cr_redirect_stdout();
    game_loop(&global, &vm);
    cr_assert_eq(vm.cycle_to_die, 20 - CYCLE_DELTA);
    cr_assert_eq(vm.live_count, 0);
    cr_assert_eq(vm.current_cycle, 0);
    cr_assert_eq(proc->is_alive, 0);
    free(proc);
}

Test(corewar_parsing, help_flag_valid)
{
    char *argv[] = {"./corewar", "-h", NULL};
    int result;

    cr_redirect_stdout();
    result = corewar(2, argv);
    cr_assert_eq(result, 0);
    cr_assert_stdout_eq_str(
        "USAGE\n"
        "./corewar [-dump nbr_cycle] [[-n prog_number] [-a load_address] "
        "prog_name] ...\n"
        "DESCRIPTION\n"
        "-dump nbr_cycle dumps the state of the virtual machine after "
        "the nbr_cycle execution\n"
        "-n prog_number sets the next program's number. By default, "
        "the first free number in the parameter order\n"
        "-a load_address sets the next program's loading address. "
        "When no address is specified, optimize the addresses so that the "
        "processes are as far away from each other as possible. "
        "The addresses are MEM_SIZE modulo.\n"
    );
}

Test(corewar_parsing, help_flag_too_many_args)
{
    char *argv[] = {"./corewar", "-h", "champion.cor", NULL};
    int result;

    cr_redirect_stderr();
    result = corewar(3, argv);
    cr_assert_eq(result, 84);
}

Test(execute_processes, fetch_valid_instruction)
{
    global_t global;
    vm_t vm;
    process_t proc;

    memset(&global, 0, sizeof(global_t));
    memset(&vm, 0, sizeof(vm_t));
    memset(&proc, 0, sizeof(process_t));
    vm.process_list = &proc;
    vm.arena[0] = 1;
    proc.pc = 0;
    proc.cycle_to_wait = 0;
    execute_processes(&global, &vm);
    cr_assert_eq(proc.current_opcode, 1);
    cr_assert_eq(proc.cycle_to_wait, op_tab[1].nbr_cycles - 1);
    cr_assert_eq(proc.pc, 0);
}

Test(execute_processes, fetch_invalid_instruction)
{
    global_t global;
    vm_t vm;
    process_t proc;

    memset(&global, 0, sizeof(global_t));
    memset(&vm, 0, sizeof(vm_t));
    memset(&proc, 0, sizeof(process_t));
    vm.process_list = &proc;
    vm.arena[0] = 42;
    proc.pc = 0;
    proc.cycle_to_wait = 0;
    execute_processes(&global, &vm);
    cr_assert_eq(proc.current_opcode, 42);
    cr_assert_eq(proc.cycle_to_wait, 0);
    cr_assert_eq(proc.pc, 1);
}

Test(execute_processes, cooldown_decrements)
{
    global_t global;
    vm_t vm;
    process_t proc;

    memset(&global, 0, sizeof(global_t));
    memset(&vm, 0, sizeof(vm_t));
    memset(&proc, 0, sizeof(process_t));
    vm.process_list = &proc;
    proc.cycle_to_wait = 5;
    proc.pc = 0;
    execute_processes(&global, &vm);
    cr_assert_eq(proc.cycle_to_wait, 4);
    cr_assert_eq(proc.pc, 0);
}

Test(execute_processes, execution_at_zero)
{
    global_t global;
    vm_t vm;
    process_t proc;

    memset(&global, 0, sizeof(global_t));
    memset(&vm, 0, sizeof(vm_t));
    memset(&proc, 0, sizeof(process_t));
    vm.process_list = &proc;
    proc.cycle_to_wait = 1;
    proc.pc = MEM_SIZE - 1;
    execute_processes(&global, &vm);
    cr_assert_eq(proc.cycle_to_wait, 0);
    cr_assert_eq(proc.pc, 0);
}

Test(my_put_nbr, positive_number)
{
    cr_redirect_stdout();
    my_put_nbr(42);
    cr_assert_stdout_eq_str("42");
}

Test(my_put_nbr, negative_number)
{
    cr_redirect_stdout();
    my_put_nbr(-84);
    cr_assert_stdout_eq_str("-84");
}

Test(my_put_nbr, zero_number)
{
    cr_redirect_stdout();
    my_put_nbr(0);
    cr_assert_stdout_eq_str("0");
}

Test(my_put_nbr, extremum_number)
{
    cr_redirect_stdout();
    my_put_nbr(-2147483648);
    cr_assert_stdout_eq_str("-2147483648");
}

Test(exec_live, valid_live_execution)
{
    global_t global;
    vm_t vm;
    process_t proc;

    memset(&global, 0, sizeof(global_t));
    memset(&vm, 0, sizeof(vm_t));
    memset(&proc, 0, sizeof(process_t));
    global.nbr_champions = 1;
    global.champions[0].prog_number = 42;
    strcpy(global.champions[0].header.prog_name, "TestChamp");
    proc.pc = 0;
    vm.arena[1] = 0;
    vm.arena[2] = 0;
    vm.arena[3] = 0;
    vm.arena[4] = 42;
    cr_redirect_stdout();
    exec_live(&global, &vm, &proc);
    cr_assert_eq(proc.is_alive, 1);
    cr_assert_eq(vm.live_count, 1);
    cr_assert_eq(proc.pc, 5);
    cr_assert_stdout_eq_str("The player 42(TestChamp) is alive.\n");
}

Test(exec_live, live_with_circular_memory)
{
    global_t global;
    vm_t vm;
    process_t proc;

    memset(&global, 0, sizeof(global_t));
    memset(&vm, 0, sizeof(vm_t));
    memset(&proc, 0, sizeof(process_t));
    global.nbr_champions = 1;
    global.champions[0].prog_number = 15;
    strcpy(global.champions[0].header.prog_name, "CircChamp");
    proc.pc = MEM_SIZE - 2;
    vm.arena[MEM_SIZE - 1] = 0;
    vm.arena[0] = 0;
    vm.arena[1] = 0;
    vm.arena[2] = 15;
    cr_redirect_stdout();
    exec_live(&global, &vm, &proc);
    cr_assert_eq(proc.is_alive, 1);
    cr_assert_eq(proc.pc, 3);
}
