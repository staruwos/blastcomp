# blastcomp

This project is a minimal, "bare metal" template for programming Mega Drive (Genesis) games and applications in C.

It's in early stages. Watch out!!

## Project Goals
- **Less External Libraries as possible**
- **Dockerized Toolchain**
- **Hardware Direct Access**

## Project Structure

| File | Description |
| :--- | :--- |
| `boot.s` | **Assembly Startup:** Sets the Vector Table, writes the SEGA security header, initializes the stack, and jumps to C. |
| `main.c` | **Game Logic:** The C entry point. Initializes the VDP registers and cycles background colors. |
| `sega.ld` | **Linker Script:** Maps code to Cartridge ROM ($000000) and variables to Console RAM ($FF0000). |
| `Dockerfile` | **Toolchain:** Builds a Linux environment with `m68k-elf-gcc` and `binutils` pre-installed. |
| `build.sh` | **Build Helper:** Automates the Docker build and compilation process. |

## Prerequisites

1.  **Docker Desktop** (or Docker Engine on Linux)
2.  **Bash** terminal (Linux, macOS, or WSL on Windows)

## How to Build

1.  **Clone the repository**

2.  **Make the script executable:**
    ```bash
    chmod +x build.sh
    ```

3.  **Run the build script:**
    ```bash
    ./build.sh
    ```

**What happens next?**
* The script checks for the `md-toolchain` Docker image. If it doesn't exist, it builds it (this takes a minute the first time).
* It mounts your current directory into the container.
* It compiles `boot.s` and `main.c` using `m68k-elf-gcc`.
* It links them using `sega.ld`.
* It extracts the raw binary to create **`game.bin`**.

## Technical Details

### The Memory Map
The linker script (`sega.ld`) enforces the standard Genesis memory map:
* **ROM:** `0x000000` to `0x3FFFFF` (4MB Cartridge space)
* **RAM:** `0xFF0000` to `0xFFFFFF` (64KB Work RAM)

### VDP Initialization
Unlike modern PCs, the Mega Drive VDP is not initialized by an OS. The `main.c` includes a `vdp_init()` function that manually sends 19 commands to the VDP Control Port to:
1.  Enable the Display (Reg 1, Bit 6).
2.  Set the resolution to 320x224 (H40 mode).
3.  Define memory locations for Scroll Planes and Sprites.

### Compilation Flags
We use specific GCC flags to generate ROM-compatible code:
* `-m68000`: Target the specific M68k CPU found in the console.
* `-ffreestanding`: Tells GCC there is no Standard C Library (no `printf`, no `stdio.h`).
* `-O2`: Optimization level 2.

## License
This project is open source. Do whatever you want.
