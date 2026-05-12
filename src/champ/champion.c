/*
** EPITECH PROJECT, 2026
** champion.c
** File description:
** Champion parsing and validation
*/

#include "../../include/corewar.h"

static int read_and_validate_header(FILE *fd,
    header_t *header,
    const char *filepath)
{
    if (fread(header, sizeof(header_t), 1, fd) != 1) {
        fprintf(stderr, "Error: File %s is too short or invalid.\n", filepath);
        return 84;
    }
    header->magic = swap_int32(header->magic);
    header->prog_size = swap_int32(header->prog_size);
    if (header->magic != COREWAR_EXEC_MAGIC) {
        fprintf(stderr, "Error: %s is not a valid corewar file.\n", filepath);
        return 84;
    }
    if (header->prog_size < 0 || header->prog_size > MEM_SIZE / 6) {
        fprintf(stderr, "Error: Program size of %s is invalid.\n", filepath);
        return 84;
    }
    return 0;
}

static int check_trailing_bytes(FILE *fd, const char *filepath)
{
    uint8_t dummy;

    if (fread(&dummy, 1, 1, fd) > 0) {
        fprintf(stderr, "Error: File %s contains trailing superfluous bytes.\n",
            filepath);
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
        perror("Error: malloc failed");
        return 84;
    }
    read_bytes = fread(champ->code, 1, champ->header.prog_size, fd);
    if (read_bytes != (size_t)champ->header.prog_size) {
        fprintf(stderr, "Error: File %s is truncated.\n", filepath);
        return 84;
    }
    if (check_trailing_bytes(fd, filepath) != 0) {
        return 84;
    }
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
        fprintf(stderr, "Error: Cannot open file %s\n", filepath);
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
