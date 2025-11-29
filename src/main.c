#include "types.h"
#include "input.h"
#include "vdp.h"

// A simple 8x8 Smiley Face Tile
const u32 smiley_tile[] = {
    0x00111100, // ..XXXX..
    0x01000010, // .X....X.
    0x10100101, // X.X..X.X (Eyes)
    0x10000001, // X......X
    0x10100101, // X.X..X.X (Smile corners)
    0x10011001, // X..XX..X (Smile)
    0x01000010, // .X....X.
    0x00111100  // ..XXXX..
};

void main() {
    vdp_init();
    input_init();

    // 1. Setup Palette
    // Color 1 will be our sprite color (White)
    vdp_set_color(1, 0x0EEE); 
    vdp_set_color(0, 0x0000); 

    // 2. Load the tile into VRAM Index 1
    vdp_load_tiles(smiley_tile, 1, 1);

    // 3. Sprite Coordinates
    // IMPORTANT: VDP Coordinates start at 128 (0x80).
    // 0x80, 0x80 is the top-left pixel of the screen.
    u16 x = 0x80 + 140; // Center X
    u16 y = 0x80 + 100; // Center Y

    while(1) {
        // Read Input
        u16 joy = input_read();

        // Move Sprite
        if (joy & BUTTON_UP)    y--;
        if (joy & BUTTON_DOWN)  y++;
        if (joy & BUTTON_LEFT)  x--;
        if (joy & BUTTON_RIGHT) x++;

        // Draw Sprite
        // ID: 0, Pos: x,y, Tile: 1
        vdp_set_sprite(0, x, y, 1);

        // Delay
        volatile int i;
        for(i=0; i<500; i++) __asm__("nop");
    }
}