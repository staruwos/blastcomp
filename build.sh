#!/bin/bash

IMAGE_NAME="md-toolchain"
CONTAINER_WORK_DIR="/project"
SRC_DIR="src"
BUILD_DIR="build"
SCRIPT_FILE="build_rom.sh"

# 1. Check if the Docker image exists, if not, build it
if [[ "$(docker images -q $IMAGE_NAME 2> /dev/null)" == "" ]]; then
    echo "Building Docker image ($IMAGE_NAME)... This may take a minute."
    docker build -t $IMAGE_NAME .
fi

echo "--== blastcomp ==--"

# 2. Run the compilation commands inside the container
# We map the current directory $(pwd) to /project in the container
docker run --rm -v "$(pwd):$CONTAINER_WORK_DIR" $IMAGE_NAME /bin/bash $SCRIPT_FILE 
