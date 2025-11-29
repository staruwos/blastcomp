#ifndef VDP_H
#define VDP_H

#include "types.h"

// VDP Hardware Ports
// We use volatile to ensure the compiler writes to the actual hardware address
#define VDP_DATA    (*(vu16 *)0xC00000)
#define VDP_CTRL    (*(vu32 *)0xC00004)

// Helper Macros
// Set VDP to write to Color RAM (CRAM) at a specific index (0-63)
// Logic: 0xC0000000 is the command base, shifted address bits needed.
#define VDP_SETUP_CRAM_WRITE(index)  (VDP_CTRL = (0xC0000000 | ((u32)(index & 0x3F) << 16)))

// VDP Commands
// To write to VRAM, the command bits are different than CRAM
// Format: 01AAAAAA AAAAAAAA 00000000 00AAAA00 (Simplified)
#define VDP_VRAM_WRITE(addr) (VDP_CTRL = (0x40000000 | ((u32)(addr) & 0x3FFF) << 16) | ((u32)(addr) >> 14))

// Initialization function
void vdp_init();

// Helper to set a single color (Palette index 0-63, Color 0x0BGR)
void vdp_set_color(u8 index, u16 color);

// Load graphical tiles into VRAM
// tile_data: Array of hex values
// index: Which tile ID to start at (0-2047)
// num_tiles: How many 8x8 tiles to load
void vdp_load_tiles(const u32 *tile_data, u16 index, u16 num_tiles);

// Update a sprite in the Sprite Attribute Table (SAT)
// id: Sprite ID (0-79)
// x, y: Screen position
// tile_index: Which graphics tile to use
void vdp_set_sprite(u8 id, u16 x, u16 y, u16 tile_index);


#endif