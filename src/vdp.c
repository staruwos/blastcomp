#include "vdp.h"

// Standard VDP Register Values for a 320x224 display (H40 Mode)
const u8 vdp_init_values[] = {
    0x04, // 00: Mode Control 1
    0x54, // 01: Mode Control 2 (Bit 6=Disp ON, Bit 5=V-Int OFF)
    0x30, // 02: Plane A Address (0xC000)
    0x00, // 03: Window Address  (0x0000)
    0x07, // 04: Plane B Address (0xE000)
    0x7C, // 05: Sprite Address  (0xF800)
    0x00, // 06: Sprite Pattern Generator Base
    0x00, // 07: Background Color
    0x00, // 08: Unused
    0x00, // 09: Unused
    0x01, // 10: H-Int Counter
    0x00, // 11: Mode Control 3
    0x81, // 12: Mode Control 4 (H40 Mode)
    0x3F, // 13: H-Scroll Table
    0x00, // 14: Unused
    0x02, // 15: Auto Increment
    0x01, // 16: Plane Size
    0x00, // 17: Window H
    0x00  // 18: Window V
};

void vdp_init() {
    int i;
    for(i = 0; i < 19; i++) {
        // To write a register, send 0x8000 | (RegIndex << 8) | Value
        // Since we are writing to a 32-bit Control Port, it handles 16-bit writes fine
        // as long as they are on the high word, but standard practice is writing 16-bits.
        u16 cmd = 0x8000 | (i << 8) | vdp_init_values[i];
        
        // We cast the 32-bit port to 16-bit for register setting 
        // to avoid writing garbage to the lower half, 
        // though writing to the full 32-bit port works if data is formatted correctly.
        // For safety/clarity, we write to the Control Port address as a 16-bit ptr here:
        *(vu16 *)0xC00004 = cmd;
    }
}

void vdp_set_color(u8 index, u16 color) {
    VDP_SETUP_CRAM_WRITE(index << 1);
    VDP_DATA = color;
}

void vdp_load_tiles(const u32 *tile_data, u16 index, u16 num_tiles) {
    // Each tile is 32 bytes (8x8 pixels, 4 bits per pixel)
    // VRAM address for tile N is N * 32
    u32 vram_addr = index * 32;
    
    // Set VDP to write to VRAM at this address
    VDP_VRAM_WRITE(vram_addr);

    // Pump the data into the data port
    // We cast to u32 so we can write 4 bytes at a time (slightly faster)
    u32 i;
    for(i = 0; i < num_tiles * 8; i++) { // 8 longs per tile (32 bytes)
        *(vu32 *)0xC00000 = tile_data[i];
    }
}

void vdp_set_sprite(u8 id, u16 x, u16 y, u16 tile_index) {
    // The Sprite Attribute Table (SAT) starts at 0xF800 (set in init reg 5)
    // Each sprite takes 8 bytes.
    u32 sat_addr = 0xF800 + (id * 8);

    VDP_VRAM_WRITE(sat_addr);

    // Word 1: Y Position
    VDP_DATA = y;
    
    // Word 2: Link (0) and Size (1x1 tile = 0000)
    // Format: 0000 (Size) 0000 (Link)
    // Size bits: 00 = 1x1, 01 = 1x2, 10 = 2x1, 11 = 2x2...
    // We hardcode size to 1x1 (0x0) and Link to 0 (Last sprite) for now
    VDP_DATA = 0x0000;

    // Word 3: Priority, Palette, Flip, Tile Index
    // Format: PCCF H V TTTTTTTT
    // We just pass the tile index directly for simplicity
    VDP_DATA = tile_index;

    // Word 4: X Position
    VDP_DATA = x;
}