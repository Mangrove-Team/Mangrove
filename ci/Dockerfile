FROM debian:trixie-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
        build-essential \
        ca-certificates \
        cmake \
        git \
        libgl-dev \
        ninja-build \
        pre-commit \
        python3 \
        qt6-base-dev \
        qt6-declarative-dev \
        qt6-declarative-dev-tools \
        qt6-svg-dev \
    && rm -rf /var/lib/apt/lists/*
