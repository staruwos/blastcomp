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

// Initialization function
void vdp_init();

// Helper to set a single color (Palette index 0-63, Color 0x0BGR)
void vdp_set_color(u8 index, u16 color);

#endif