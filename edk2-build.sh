#!/bin/bash
# Copyright (c) Microsoft Corporation. All rights reserved.

# Setup edk2 workspace
cd $WORKSPACE
export PACKAGES_PATH=$PWD/edk2:$PWD/imx-edk2-platforms
export GCC5_ARM_PREFIX=arm-linux-gnueabihf-
# Note: Path to cross compiler is hardcoded
export PATH=/home/jenkins/gcc-linaro-6.4.1-2017.11-x86_64_arm-linux-gnueabihf/bin:$PATH
NUM_CPUS=$((`getconf _NPROCESSORS_ONLN` + 2))

# Print relevant environment variables
echo "WORKSPACE=${WORKSPACE}"
echo "PACKAGES_PATH=${PACKAGES_PATH}"
echo "GCC5_ARM_PREFIX=${GCC5_ARM_PREFIX}"
echo "PATH=${PATH}"
echo "NUM_CPUS=${NUM_CPUS}"
echo "Date=$(date)!"

# Build tools
. edk2/edksetup.sh --reconfig
make -C edk2/BaseTools

# Build iMX6
build -n $NUM_CPUS -a ARM -t GCC5 -p Platform/SolidRun/HummingBoardEdge_iMX6Q_2GB/HummingBoardEdge_iMX6Q_2GB.dsc
# Build iMX7
build -n $NUM_CPUS -a ARM -t GCC5 -p Platform/Compulab/ClSomImx7_iMX7D_1GB/ClSomImx7_iMX7D_1GB.dsc
