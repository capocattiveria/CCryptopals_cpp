# Sviluppo nel container Docker

Ambiente di sviluppo containerizzato **senza** `devcontainer.json`, basato su
`compose.yaml` (root del repo) che riusa `.devcontainer/Dockerfile`.
Dentro il container si programma con **neovim** e **tmux**, usando le config
dell'host.

## Prerequisiti

- Docker + Docker Compose (Docker Desktop va bene). Il daemon deve essere in
  esecuzione, altrimenti:
  `Cannot connect to the Docker daemon ... Is the docker daemon running?`
- Config dell'host montate nel container (path attesi):
  - `~/.config/nvim` — config di neovim (montata in read-write)
  - `~/.config/tmux/tmux.conf` — config di tmux (read-only)
  - `~/.ssh` — per `git push` dal container (read-only)

## Uso

```bash
# build dell'immagine (una volta sola, o dopo modifiche al Dockerfile)
docker compose build

# avvia il container in background
docker compose up -d

# entra nella shell dentro il container
docker compose exec dev bash

# a lavoro finito
docker compose down
```

Alternativa senza container persistente (shell usa e getta):

```bash
docker compose run --rm dev bash
```

## Dentro il container

Il progetto e' montato in `/app`.

```bash
make clean        # consigliato la prima volta: rimuove artefatti buildati sull'host
make run SET=1 CHALLENGE=6

tmux              # legge ~/.config/tmux/tmux.conf
nvim .            # legge ~/.config/nvim
```

Al primo avvio di `nvim` i plugin (lazy.nvim) e i parser treesitter vengono
scaricati/compilati dentro il container; restano nei volumi `nvim-*` e non
vengono riscaricati agli avvii successivi.

Toolchain gia' presente nell'immagine: `g++`, `gdb`, `make`, `bear`, `clangd`
(LSP C/C++), `git`, `ripgrep`, `fd`, `unzip`, `curl`/`wget`, `node`/`npm`,
`python3`, `go`.

LSP server per neovim gia' installati (in `PATH`):

- `clangd` — C/C++
- `pyright` — Python (via `npm -g`, richiede `python3`, presente)
- `gopls` — Go (compilato con la toolchain Go dell'immagine)

La versione di Go e' fissata in `.devcontainer/Dockerfile` (`ARG GO_VERSION`);
per cambiarla: `docker compose build --build-arg GO_VERSION=X.Y.Z`.

## Neovim: allineare la versione all'host

L'immagine installa Neovim dal tarball ufficiale. La versione e' fissata in
`.devcontainer/Dockerfile` (`ARG NVIM_VERSION`). Per cambiarla:

```bash
docker compose build --build-arg NVIM_VERSION=v0.11.3
```

## Reset dello stato di neovim

Se i plugin nel container si rompono, ricrea i volumi da zero:

```bash
docker compose down
docker volume rm cryptopals_nvim-data cryptopals_nvim-state cryptopals_nvim-cache
docker compose up -d
```

## Scelte in `compose.yaml`

- **`.:/app`** — bind mount: editi i file dall'host, compili dentro il container.
- **`user: "1000:1000"`** — gira col tuo uid host (= utente `ubuntu`
  nell'immagine), cosi' i file generati (binari, `compile_commands.json`,
  `lazy-lock.json`) non diventano di `root` e git non lamenta
  "dubious ownership". Se serve root:
  `docker compose run --rm --user root dev bash`.
- **`HOME=/home/ubuntu`** — home persistente dell'utente; sotto ci sono i mount
  di `.config/nvim`, `.config/tmux`, `.ssh`.
- **`~/.config/nvim` in read-write** — cosi' `:Lazy sync` puo' aggiornare
  `lazy-lock.json` (che e' un file versionato del tuo repo di config).
- **volumi `nvim-data` / `nvim-state` / `nvim-cache`** — stato e plugin di
  neovim vivono in volumi dedicati al container: i binari (parser treesitter,
  ecc.) sono compilati per l'immagine e non entrano in conflitto con quelli
  dell'host.
- **`TERM`** — propagato dall'host per avere i colori giusti in tmux/nvim.
- **`command: sleep infinity`** — tiene vivo il container per
  `docker compose exec`.
