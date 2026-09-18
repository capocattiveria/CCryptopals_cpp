# Sviluppo nel container Docker

Ambiente di sviluppo containerizzato basato su `compose.yaml` (root del repo,
riusa `.devcontainer/Dockerfile`). Dentro il container si programma con
**neovim** e **tmux**, usando le config dell'host.

## Prerequisiti

- Docker + Docker Compose, daemon in esecuzione
- Config dell'host montate nel container:
  - `~/.config/nvim` (read-write, cosi' `:Lazy sync` puo' aggiornare `lazy-lock.json`)
  - `~/.config/tmux/tmux.conf` (read-only)
  - `~/.ssh` (read-only, per `git push` dal container)

## Uso

```bash
docker compose build       # build immagine (una volta, o dopo modifiche al Dockerfile)
docker compose up -d       # avvia il container in background
docker compose exec dev bash   # entra nella shell
docker compose down        # a lavoro finito
```

Alternativa senza container persistente: `docker compose run --rm dev bash`.

## Dentro il container

Progetto montato in `/app`.

```bash
make clean        # consigliato la prima volta: rimuove artefatti buildati sull'host
make run SET=1 CHALLENGE=6

tmux              # legge ~/.config/tmux/tmux.conf
nvim .            # legge ~/.config/nvim
```

Toolchain nell'immagine: `g++`, `gdb`, `make`, `bear`, `clangd`, `git`,
`ripgrep`, `fd`, `node`/`npm`, `python3`, `go`, `claude` (CLI di Claude Code),
`wl-clipboard`/`xclip`.

### Claude Code

Config indipendente da quella dell'host, su volume dedicato `claude-config`
(`/home/ubuntu/.claude`). Login una tantum, sopravvive a `down && up`. Per
rifare il login da zero: `docker volume rm cryptopals_claude-config`.

### Clipboard

Richiede una sessione **Wayland** sull'host (il container riusa il socket via
`compose.yaml`): `"+y`/`"+p` in neovim e la copy-mode di tmux scrivono/leggono
la clipboard dell'host. Su X11/senza display, rimuovi il mount del socket e le
variabili `WAYLAND_DISPLAY`/`XDG_RUNTIME_DIR` da `compose.yaml` e passa a OSC
52.

### Reset stato neovim

Se i plugin si rompono:

```bash
docker compose down
docker volume rm cryptopals_nvim-data cryptopals_nvim-state cryptopals_nvim-cache
docker compose up -d
```

## Versioni (Go, Node, Neovim)

Fissate in `.devcontainer/Dockerfile` (`ARG GO_VERSION`, `ARG NODE_VERSION`,
`ARG NVIM_VERSION`). Per cambiarle: `docker compose build --build-arg NOME=X.Y.Z`.
