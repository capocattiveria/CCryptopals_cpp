FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    g++ \
    gdb \
    make \
    git \
    bear \
    clangd \
    curl \
    wget \
    ca-certificates \
    libssl-dev \
    tmux \
    ripgrep \
    fd-find \
    unzip \
    nodejs \
    npm \
    python3 \
    python3-venv \
    && ln -s "$(command -v fdfind)" /usr/local/bin/fd \
    && rm -rf /var/lib/apt/lists/*

# Neovim: stessa minor version dell'host. Bump con:
#   docker compose build --build-arg NVIM_VERSION=vX.Y.Z
ARG NVIM_VERSION=v0.11.3
RUN curl -fsSL -o /tmp/nvim.tar.gz \
      "https://github.com/neovim/neovim/releases/download/${NVIM_VERSION}/nvim-linux-x86_64.tar.gz" \
    && tar -xzf /tmp/nvim.tar.gz -C /opt \
    && ln -s /opt/nvim-linux-x86_64/bin/nvim /usr/local/bin/nvim \
    && rm /tmp/nvim.tar.gz

# Go toolchain (dal tarball ufficiale, come neovim): serve a compilare gopls
# e per lo sviluppo Go dentro il container. Bump con --build-arg GO_VERSION=X.Y.Z
ARG GO_VERSION=1.23.4
RUN curl -fsSL -o /tmp/go.tar.gz \
      "https://go.dev/dl/go${GO_VERSION}.linux-amd64.tar.gz" \
    && tar -xzf /tmp/go.tar.gz -C /opt \
    && ln -s /opt/go/bin/go /usr/local/bin/go \
    && ln -s /opt/go/bin/gofmt /usr/local/bin/gofmt \
    && rm /tmp/go.tar.gz

# LSP server usati dalla config di neovim:
#   - pyright (Python)  -> via npm, installato in /usr
#   - gopls   (Go)      -> compilato con la toolchain sopra, binario in /usr/local/bin
# GOTOOLCHAIN=auto: se gopls@latest richiede un Go piu' recente lo scarica da solo.
RUN npm install -g pyright \
    && GOBIN=/usr/local/bin GOPATH=/tmp/go GOCACHE=/tmp/go-build GOTOOLCHAIN=auto \
       go install golang.org/x/tools/gopls@latest \
    && rm -rf /tmp/go /tmp/go-build

# HOME persistente per l'utente host (uid 1000 = utente "ubuntu" in ubuntu:24.04).
# Le dir di stato/plugin di nvim sono volumi "named": precrearle qui le fa
# inizializzare con i permessi giusti (Docker copia owner/mode dell'immagine
# in un volume vuoto al primo avvio).
RUN mkdir -p \
      /home/ubuntu/.config \
      /home/ubuntu/.local/share/nvim \
      /home/ubuntu/.local/state/nvim \
      /home/ubuntu/.cache/nvim \
    && chown -R ubuntu:ubuntu /home/ubuntu

WORKDIR /app
