FROM debian:bookworm-slim

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Update and install only the exact cross-compiler tools we need.
# --no-install-recommends prevents downloading bloated, unnecessary optional packages.
RUN apt-get update && apt-get install -y --no-install-recommends \
    gcc-m68k-linux-gnu \
    binutils-m68k-linux-gnu \
    make \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# Create a working directory
WORKDIR /project

# By default, this container does nothing.
# The user must pass commands via the helper script.
CMD ["/bin/bash"]