/*
** EPITECH PROJECT, 2026
** champion.c
** File description:
** Champion parsing and validation
*/

#include "../../include/corewar.h"

static void print_err(const char *pre, const char *path, const char *suf)
{
    const char *strs[3] = {pre, path, suf};

    for (int j = 0; j < 3; j++) {
        for (int i = 0; strs[j] && strs[j][i] != '\0'; i++)
            write(2, &strs[j][i], 1);
    }
}

static int read_and_validate_header(FILE *fd,
    header_t *header,
    const char *filepath)
{
    if (fread(header, sizeof(header_t), 1, fd) != 1) {
        print_err("Error: File ", filepath, " is too short or invalid.\n");
        return 84;
    }
    header->magic = swap_int32(header->magic);
    header->prog_size = swap_int32(header->prog_size);
    if (header->magic != COREWAR_EXEC_MAGIC) {
        print_err("Error: ", filepath, " is not a valid corewar file.\n");
        return 84;
    }
    if (header->prog_size < 0 || header->prog_size > MEM_SIZE / 6) {
        print_err("Error: Program size of ", filepath, " is invalid.\n");
        return 84;
    }
    return 0;
}

static int check_trailing_bytes(FILE *fd, const char *filepath)
{
    uint8_t dummy;

    if (fread(&dummy, 1, 1, fd) > 0) {
        print_err("Error: File ", filepath,
            " contains trailing superfluous bytes.\n");
        return 84;
    }
    return 0;
}

static int read_payload(FILE *fd, champion_t *champ, const char *filepath)
{
    size_t read_bytes;

    if (champ->header.prog_size == 0) {
        champ->code = NULL;
        return 0;
    }
    champ->code = malloc(sizeof(uint8_t) * champ->header.prog_size);
    if (champ->code == NULL) {
        print_err("Error: malloc failed\n", NULL, NULL);
        return 84;
    }
    read_bytes = fread(champ->code, 1, champ->header.prog_size, fd);
    if (read_bytes != (size_t)champ->header.prog_size) {
        print_err("Error: File ", filepath, " is truncated.\n");
        return 84;
    }
    if (check_trailing_bytes(fd, filepath) != 0)
        return 84;
    return 0;
}

void free_champion(champion_t *champ)
{
    if (champ != NULL && champ->code != NULL) {
        free(champ->code);
        champ->code = NULL;
    }
}

int load_champion(champion_t *champ, const char *filepath)
{
    FILE *fd = fopen(filepath, "r");

    if (fd == NULL) {
        print_err("Error: Cannot open file ", filepath, "\n");
        return 84;
    }
    champ->code = NULL;
    if (read_and_validate_header(fd, &champ->header, filepath) != 0) {
        fclose(fd);
        return 84;
    }
    if (read_payload(fd, champ, filepath) != 0) {
        free_champion(champ);
        fclose(fd);
        return 84;
    }
    fclose(fd);
    return 0;
}
