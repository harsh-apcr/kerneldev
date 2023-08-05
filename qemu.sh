#!/bin/sh
set -e
export TARGET=i686-elf
export PATH="$HOME/opt/cross/bin:$PATH"
. ./build.sh

qemu-system-$(./target-triplet-to-arch.sh $HOST) -kernel sysroot/boot/myos.kernel
