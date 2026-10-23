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
- `cmake` (>= 3.20) and `ninja`
- `libssl-dev` (OpenSSL headers, the library links against `libcrypto`)

### Optional

- Docker + Docker Compose — see [DEV.md](DEV.md) for a containerized dev
  environment (neovim + tmux + Claude Code CLI, no local toolchain required)

## Build

From the repo root:

```bash
cmake --preset debug && cmake --build --preset debug       # debug build
cmake --preset release && cmake --build --preset release   # release build
```

All challenges are built into a single binary, `build/<preset>/cryptopals`.
Pick the challenge at runtime, from the repo root (assets are read from `./assets`):

```bash
./build/debug/cryptopals s01c06
./build/debug/cryptopals          # lists available challenges
```

To add a challenge, include its header in `Challenges/main.cpp` and add a line
to the `challenges` table. `compile_commands.json` is generated automatically
in `build/<preset>/` (used by clangd via `.clangd`).

## Development environment

See [DEV.md](DEV.md) for how to work inside the provided Docker dev container
(neovim, tmux, and Claude Code preconfigured, no need to install a local
toolchain).
