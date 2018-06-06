# Copyright (c) Microsoft Corporation. All rights reserved.

# Use Ubuntu 16.04-Xenial (WSL configuration)
FROM ubuntu:16.04

# Get build tools
RUN apt-get update && apt-get install -y \
    bc \
    bison \
    build-essential \
    device-tree-compiler \
    flex \
    git \
    iasl \
    python \
    python-crypto \
    python-dev \
    python-wand \
    swig \
    u-boot-tools \
    uuid-dev \
    wget \
  && rm -rf /var/lib/apt/lists/*

RUN useradd jenkins --shell /bin/bash --create-home
USER jenkins

WORKDIR /home/jenkins
RUN wget https://releases.linaro.org/components/toolchain/binaries/6.4-2017.11/arm-linux-gnueabihf/gcc-linaro-6.4.1-2017.11-x86_64_arm-linux-gnueabihf.tar.xz
RUN tar xf gcc-linaro-6.4.1-2017.11-x86_64_arm-linux-gnueabihf.tar.xz && rm gcc-linaro-6.4.1-2017.11-x86_64_arm-linux-gnueabihf.tar.xz
COPY edk2-build.sh .
