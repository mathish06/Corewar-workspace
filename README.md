# ⚔️ Corewar

> A virtual machine executing "warrior" programs fighting for memory supremacy — a C implementation of the classic Core War battle arena.

[![Language](https://img.shields.io/badge/language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![Build](https://img.shields.io/badge/build-Makefile-orange.svg)](#building)
[![Tests](https://img.shields.io/badge/tests-Criterion-purple.svg)](#running-tests)

---

## 📖 What is Corewar?

**Corewar** is a programming game in which players write programs — called *champions* — in a low-level language called *Redcode*. These champions are loaded into a shared circular memory arena and battle each other by attempting to survive and eliminate opponents.

This project is a full **virtual machine (VM)** written in C that:
- Loads and validates `.cor` champion binary files
- Runs a simulated arena (6 KB circular memory)
- Executes all Corewar instructions across multiple concurrent processes
- Declares the last champion to call `live` the winner

---

## 🏗️ Project Structure

```
corewar/
├── include/
│   ├── corewar.h          # Main header — all function prototypes & macros
│   ├── corwar_struct.h    # Core data structures (vm_t, process_t, champion_t, global_t)
│   └── op.h               # Op-table definitions, argument types, header format
├── src/
│   ├── main.c             # Entry point
│   ├── op.c               # Instruction table (op_tab[])
│   ├── arena/             # Memory arena initialization & dump
│   ├── champ/             # Champion loading, endian conversion, arena placement
│   ├── core/              # Game loop, process execution dispatcher
│   ├── instructions/      # All VM instructions (live, zjmp, fork, ld, st, add, …)
│   ├── parsing/           # CLI argument parsing (-n, -dump flags)
│   └── tools/             # Coding byte decoder, utilities
├── lib/
│   └── my/                # Custom C standard-library reimplementations
├── tests/
│   └── unit_tests.c       # Criterion unit tests
└── Makefile
```

---

## ⚙️ Supported Instructions

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| `0x01` | `live`   | Declare the champion is alive |
| `0x02` | `ld`     | Load value into register |
| `0x03` | `st`     | Store register to memory/register |
| `0x04` | `add`    | Add two registers |
| `0x05` | `sub`    | Subtract two registers |
| `0x06` | `and`    | Bitwise AND |
| `0x07` | `or`     | Bitwise OR |
| `0x08` | `xor`    | Bitwise XOR |
| `0x09` | `zjmp`   | Jump if carry flag is set |
| `0x0a` | `ldi`    | Load indirect (indexed) |
| `0x0b` | `sti`    | Store indirect (indexed) |
| `0x0c` | `fork`   | Spawn a new process |
| `0x0d` | `lld`    | Long load (no IDX_MOD) |
| `0x0e` | `lldi`   | Long load indirect |
| `0x0f` | `lfork`  | Long fork (no IDX_MOD) |
| `0x10` | `aff`    | Print ASCII character of register |

---

## 🚀 Building

> **Requirement:** A C compiler compatible with `epiclang` (or standard `gcc`/`clang`). The `Criterion` library is only needed for tests.

```bash
# Clone the repository
git clone https://github.com/mathish06/Corewar-workspace.git
cd Corewar-workspace

# Build the VM binary
make

# Clean build artifacts
make fclean

# Rebuild from scratch
make re
```

The compiled binary will be at `./corewar`.

---

## 🎮 Usage

```bash
./corewar [-dump <cycle>] [-n <number> <champion.cor>] ... <champion.cor>
```

### Flags

| Flag | Description |
|------|-------------|
| `-dump <N>` | Dump the arena memory after cycle N and exit |
| `-n <N> <file>` | Assign player number N to the given champion |

### Examples

```bash
# Run two champions against each other
./corewar bomber.cor cloner.cor

# Run four champions
./corewar le_roc.cor le_fuyard.cor le_savant.cor vampire.cor

# Dump arena memory at cycle 100 and exit
./corewar -dump 100 bomber.cor cloner.cor

# Assign explicit player numbers
./corewar -n 1 bomber.cor -n 2 vampire.cor
```

---

## 🧪 Running Tests

Unit tests use the [Criterion](https://github.com/Snaipe/Criterion) testing framework.

```bash
# Run all unit tests with coverage report
make tests_run

# Generate HTML coverage report (opens in browser)
make coverage
```

---

## 🏆 Included Champions

Several pre-compiled `.cor` champion files are provided to test the VM:

| File | Description |
|------|-------------|
| `bomber.cor` | Classic bombing strategy |
| `cloner.cor` | Clones itself across memory |
| `vampire.cor` | Aggressive memory takeover |
| `le_roc.cor` | Solid defensive champion |
| `le_fuyard.cor` | Evasive runner strategy |
| `le_savant.cor` | Smart calculation-based warrior |
| `l_hydre.cor` | Hydra — multi-process champion |
| `pdd.cor` | Classic PDD-style warrior |

---

## 🛠️ Arena Constants

| Constant | Value | Description |
|----------|-------|-------------|
| `MEM_SIZE` | 6144 bytes | Total arena memory (6 KB circular) |
| `REG_NUMBER` | 16 | Number of registers per process |
| `CYCLE_TO_DIE` | 1536 | Cycles before a process is declared dead |
| `CYCLE_DELTA` | 5 | Reduction applied each period |
| `NBR_LIVE` | 40 | Min live calls to survive a period |
| `IDX_MOD` | 512 | Index modulo for short addressing |

---

## 👥 Authors

Developed as part of the **Epitech** curriculum (CPE-200 — Core War project).

- **mathish06** — [@mathish06](https://github.com/mathish06)

---

## 📄 License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.
