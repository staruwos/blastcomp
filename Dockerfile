# Use Ubuntu as base because it has m68k packages in standard repos
FROM ubuntu:22.04

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Update and install the cross-compiler and build tools
RUN apt-get update && apt-get install -y \
    gcc-m68k-linux-gnu \
    binutils-m68k-linux-gnu \
    make \
    && rm -rf /var/lib/apt/lists/*

# Create a working directory
WORKDIR /project

# By default, this container does nothing.
# The user must pass commands via the helper script.
CMD ["/bin/bash"]
