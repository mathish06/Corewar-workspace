##
## EPITECH PROJECT, 2026
## Corewar
## File description:
## Makefile for Corewar VM
##

MAKEFLAGS += --no-print-directory

CC      =   epiclang

PROJECT =   COREWAR
NAME    =   corewar
LIB_NAME =  libmy.a

# 2. SOURCES

SRC     =   src/main.c 							\
			src/arena/arena.c 					\
			src/champ/endian.c 					\
			src/champ/champion.c				\
			src/core/corewar.c					\
			src/tools/tools.c					\
			src/champ/prog_champ.c				\
			src/core/game_loop.c


LIB_SRC =   lib/my/my_strcmp.c					\
			lib/my/my_getnbr.c					\
			lib/my/my_putchar.c					\
			lib/my/my_putstr.c

# 3. OBJETS
TESTS_SRC = tests/unit_tests.c
OBJ     =   $(SRC:.c=.o)
LIB_OBJ =   $(LIB_SRC:.c=.o)

# 4. FLAGS
CFLAGS  =   -I./include -Wall -Wextra -g
LDFLAGS =   -L. -lmy
TESTS_FLAGS = --coverage -lcriterion

# ─── Macro: Fancy Header ─────────────────────────────────────────────────
define pretty_header
	@echo "$(BOLD)$(H_PURPLE)╔═══════════════════════════════════════════════════════════════╗$(END)"
	@echo "$(BOLD)$(H_PURPLE)║$(END)$(BOLD)$(WHITE)$(1)$(END)"
	@echo "$(BOLD)$(H_PURPLE)╚═══════════════════════════════════════════════════════════════╝$(END)"
endef

# ─── Pattern Rules (Esthétique de compilation) ───────────────────────────
%.o: %.c
	@printf "$(H_BLUE)⚡ $(BOLD)Compiling:$(END) $(H_YELLOW)%-45s$(END) $(H_GREEN)[OK]$(END)\r" $<
	@$(CC) $(CFLAGS) -c $< -o $@

# ─── Rules ───────────────────────────────────────────────────────────────
.PHONY: all clean fclean re tests_run coverage help banner setup

all: banner $(NAME)

$(NAME): $(LIB_NAME) $(OBJ)
	@printf "\n"
	$(call pretty_header, 🚀 Linking binary : $(NAME) 🚀)
	@$(CC) -o $(NAME) $(OBJ) $(LDFLAGS)
	@echo "$(BOLD)$(H_GREEN)✅ $(PROJECT) successfully compiled!$(END)"

$(LIB_NAME): $(LIB_OBJ)
	@printf "\n"
	$(call pretty_header, 📚 Creating library : $(LIB_NAME) 📚)
	@ar rc $(LIB_NAME) $(LIB_OBJ)
	@mkdir -p lib/my
	@cp $(LIB_NAME) lib/my/
	@echo "$(BOLD)$(H_GREEN)✅ Library compiled!$(END)"

banner:
	@echo "$(H_CYAN)                                                              $(END)"
	@echo "$(H_CYAN)   ██████╗ ██████╗ ██████╗ ███████╗██╗    ██╗ █████╗ ██████╗  $(END)"
	@echo "$(H_CYAN)  ██╔════╝██╔═══██╗██╔══██╗██╔════╝██║    ██║██╔══██╗██╔══██╗ $(END)"
	@echo "$(H_CYAN)  ██║     ██║   ██║██████╔╝█████╗  ██║ █╗ ██║███████║██████╔╝ $(END)"
	@echo "$(H_CYAN)  ██║     ██║   ██║██╔══██╗██╔══╝  ██║███╗██║██╔══██║██╔══██╗ $(END)"
	@echo "$(H_CYAN)  ╚██████╗╚██████╔╝██║  ██║███████╗╚███╔███╔╝██║  ██║██║  ██║ $(END)"
	@echo "$(H_CYAN)   ╚═════╝ ╚═════╝ ╚═╝  ╚═╝╚══════╝ ╚══╝╚══╝ ╚═╝  ╚═╝╚═╝  ╚═╝ $(END)"
	@echo "$(H_CYAN)                                                              $(END)"

clean:
	$(call pretty_header, 🧹 Cleaning object files 🧹)
	@find . -name "*.o" -delete
	@rm -f *.gcno *.gcda
	@rm -f vgcore.*
	@rm -f *~

fclean: clean
	$(call pretty_header, 🗑️ Full clean: objects binary tests 🗑️)
	@rm -f $(NAME)
	@rm -f $(LIB_NAME)
	@rm -f lib/my/$(LIB_NAME)
	@rm -f unit_tests
	@rm -f coverage.html

re: fclean all

# ─── Tests Rules ─────────────────────────────────────────────────────────
tests_run: fclean $(LIB_NAME)
	$(call pretty_header, 🧪 Compiling and running unit tests 🧪)
	@gcc -o unit_tests $(filter-out src/main.c, $(SRC)) $(TESTS_SRC) $(CFLAGS) $(LDFLAGS) $(TESTS_FLAGS)
	@./unit_tests
	@gcovr --exclude tests/
	@gcovr --txt-metric branch --exclude tests/

coverage: tests_run
	@gcovr --html-details -o coverage.html --exclude tests/
	@xdg-open coverage.html || echo "Coverage generated in coverage.html"

# ─── Setup ───────────────────────────────────────────────────────────────
setup:
	@mkdir -p .githooks
	@git config core.hooksPath .githooks
	@echo "$(BOLD)$(H_GREEN) Git hooks configurés !$(END)"

# ─── Styles & Colors ─────────────────────────────────────────────────────
END = \033[0m
BOLD = \033[1m
H_GREEN = \033[92m
H_YELLOW = \033[93m
H_BLUE = \033[94m
H_PURPLE = \033[95m
H_CYAN = \033[96m
WHITE = \033[37m
CYAN = \033[36m
YELLOW = \033[33m