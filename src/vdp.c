#include "vdp.h"

// Standard VDP Register Values for a 320x224 display (H40 Mode)
const u8 vdp_init_values[] = {
    0x04, // 00: Mode Control 1
    0x74, // 01: Mode Control 2 (Bit 6 = Display Enable, Bit 5 = VBlank Int)
    0x30, // 02: Plane A Address (0xC000)
    0x00, // 03: Window Address  (0x0000)
    0x07, // 04: Plane B Address (0xE000)
    0x78, // 05: Sprite Address  (0xF800)
    0x00, // 06: Sprite Pattern Generator Base
    0x00, // 07: Background Color (Palette 0, Color 0)
    0x00, // 08: Unused
    0x00, // 09: Unused
    0x01, // 10: H-Int Counter
    0x00, // 11: Mode Control 3
    0x81, // 12: Mode Control 4 (H40 Mode = 320px wide)
    0x3F, // 13: H-Scroll Table Address (0xFC00)
    0x00, // 14: Unused
    0x02, // 15: Auto Increment (IMPORTANT: Set to 2 for sequential writes)
    0x01, // 16: Plane Size (64x32 tiles)
    0x00, // 17: Window H Pos
    0x00  // 18: Window V Pos
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
    VDP_SETUP_CRAM_WRITE(index);
    VDP_DATA = color;
}