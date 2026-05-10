/*
** EPITECH PROJECT, 2026
** champion.c
** File description:
** Champion parsing and validation
*/

#include "../../include/corewar.h"

static void print_champion_info(const header_t *h)
{
    printf("Champion '%s' loaded successfully! (Size: %d bytes)\n",
        h->prog_name, h->prog_size);
    printf("Comment: %s\n", h->comment);
}

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

int load_champion_header(const char *filepath)
{
    FILE *fd = fopen(filepath, "r");
    header_t header;

    if (fd == NULL) {
        fprintf(stderr, "Error: Cannot open file %s\n", filepath);
        return 84;
    }
    if (read_and_validate_header(fd, &header, filepath) != 0) {
        fclose(fd);
        return 84;
    }
    print_champion_info(&header);
    fclose(fd);
    return 0;
}
