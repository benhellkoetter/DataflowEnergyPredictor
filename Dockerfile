FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    wget \
    ca-certificates \
    python3 \
    libsqlite3-dev \
    zlib1g-dev \
    libncurses5-dev \
    libboost-all-dev \
    llvm-15 \
    clang-15 \
    libclang-15-dev && \
    wget "https://github.com/Kitware/CMake/releases/download/v3.25.3/cmake-3.25.3-linux-x86_64.sh" -O /tmp/cmake.sh && \
    chmod +x /tmp/cmake.sh && \
    /tmp/cmake.sh --prefix=/usr/local --exclude-subdir --skip-license && \
    rm /tmp/cmake.sh && \
    rm -rf /var/lib/apt/lists/*

RUN update-alternatives --install /usr/bin/clang clang /usr/bin/clang-15 100 && \
    update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-15 100 && \
    update-alternatives --install /usr/bin/llvm-config llvm-config /usr/bin/llvm-config-15 100

WORKDIR /usr/src
RUN git clone --recurse-submodules https://github.com/secure-software-engineering/phasar.git

WORKDIR /usr/src/phasar/build

RUN CC=clang-15 CXX=clang++-15 cmake .. \
      -DPHASAR_BUILD_LLVM_INTERFACES=ON \
      -DCMAKE_BUILD_TYPE=Release \
      -DPHASAR_USE_LLVM_CONFIG=ON \
      -DCMAKE_CXX_STANDARD=20

RUN make -j$(nproc) && make install

CMD ["sleep", "infinity"]