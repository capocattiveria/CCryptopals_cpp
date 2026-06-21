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
    libssl-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app