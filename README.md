# Cryptopals solutions

Solutions to the [Cryptopals crypto challenges](https://cryptopals.com/) in C++.

The repo is organized as:

- `CCrypto/` — a small shared library (`libCCrypto`) with the crypto primitives
  and utilities used by the challenges (hex/base64 conversion, XOR, English-text
  scoring, hamming distance, PKCS#7 padding, ...).
- `Challenges/setN/` — one `.cpp`/`.h` pair per challenge (e.g. `S01C06` = Set 1,
  Challenge 6), plus a shared `main.cpp` that dispatches to the right challenge
  based on the `SET`/`CHALLENGE` build variables.
- `assets/` — sample data files used by the challenges (English text sample,
  ciphertexts, ...).

## Progress

| Set | Challenges done |
|-----|------------------|
| 1   | 1–8 |
| 2   | 1 |

## Requirements

- `g++` with C++23 support
- `make`
- `libssl-dev` (OpenSSL headers, the library links against `libcrypto`)

### Optional

- `bear` — to (re)generate `compile_commands.json` for clangd/LSP support
- Docker + Docker Compose — see [DEV.md](DEV.md) for a containerized dev
  environment (neovim + tmux + Claude Code CLI, no local toolchain required)

## Build

From the repo root:

```bash
make
```

This builds `CCrypto` and, by default, Set 1 / Challenge 1 in debug mode.

Pick a specific challenge and build type with variables:

```bash
make SET=1 CHALLENGE=6              # debug build (default)
make release SET=2 CHALLENGE=1      # release build
```

The resulting binary is placed at:

```
bin/debug/cryptopalsS<SET>C<CHALLENGE>     # BUILD=debug (default)
bin/release/cryptopalsS<SET>C<CHALLENGE>   # BUILD=release
```

Run it directly, e.g.:

```bash
make SET=1 CHALLENGE=6 && ./bin/debug/cryptopalsS1C6
```

## Other Makefile targets

```bash
make list              # list available challenge sets
make clean             # remove build artifacts (challenges + CCrypto)
make clean-all         # clean plus remove build/bin directories
make compile_commands  # regenerate compile_commands.json with bear
make help              # show all targets and variables
```

## Development environment

See [DEV.md](DEV.md) for how to work inside the provided Docker dev container
(neovim, tmux, and Claude Code preconfigured, no need to install a local
toolchain).
