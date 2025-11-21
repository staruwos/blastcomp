#!/bin/bash

IMAGE_NAME="md-toolchain"
CONTAINER_WORK_DIR="/project"

# 1. Check if the Docker image exists, if not, build it
if [[ "$(docker images -q $IMAGE_NAME 2> /dev/null)" == "" ]]; then
    echo "Building Docker image ($IMAGE_NAME)... This may take a minute."
    docker build -t $IMAGE_NAME .
fi

echo "--== blastcomp ==--"

# 2. Run the compilation commands inside the container
# We map the current directory $(pwd) to /project in the container
docker run --rm -v "$(pwd):$CONTAINER_WORK_DIR" $IMAGE_NAME /bin/bash -c "
    # Compile Assembly Boot Code
    echo 'Compiling Boot Code...'
    m68k-linux-gnu-gcc -c boot.s -o boot.o

    # Compile C Code
    # -m68000: Target the specific CPU
    # -ffreestanding: No standard library (stdio.h, etc)
    # -O2: Optimize code
    echo 'Compiling C Code...'
    m68k-linux-gnu-gcc -m68000 -ffreestanding -O2 -c main.c -o main.o

    # Link them together using the linker script
    echo 'Linking...'
    m68k-linux-gnu-ld -T sega.ld -o output.elf boot.o main.o

    # Extract the binary machine code to create the .bin ROM
    echo 'Building .bin...'
    m68k-linux-gnu-objcopy -O binary output.elf game.bin

    echo 'Build Complete: game.bin created.'
"