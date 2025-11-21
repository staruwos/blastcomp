// Define integer types for the 68000 (Big Endian)
typedef volatile unsigned short vu16;
typedef volatile unsigned long  vu32;

// VDP Ports
#define VDP_DATA    (*(vu16 *)0xC00000)
#define VDP_CTRL    (*(vu32 *)0xC00004)

// Standard VDP Register Values for a 320x224 display
const unsigned char vdp_init_values[] = {
    0x04, // 00: Mode Control 1
    0x74, // 01: Mode Control 2 (Bit 6 = Display Enable!)
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
        unsigned short cmd = 0x8000 | (i << 8) | vdp_init_values[i];
        VDP_CTRL = cmd;
    }
}

void delay(int count) {
    volatile int i;
    for(i = 0; i < count; i++) {
        __asm__("nop");
    }
}

void main() {
    // 1. Initialize the VDP (Turn screen on, set resolution)
    vdp_init();

    // 2. Set Write Target to CRAM (Color RAM), Index 0 (Background color)
    // Command: 0xC0000000 | (Address << 16)
    // This tells the VDP: "The next data writes go to CRAM address 0"
    VDP_CTRL = 0xC0000000; 

    // 3. Write a bright color immediately to prove code is running
    VDP_DATA = 0x00E0; // Green

    unsigned short color = 0;

    while(1) {
        // Reset VDP write target to CRAM Index 0
        VDP_CTRL = 0xC0000000;
        
        // Write the new color
        VDP_DATA = color;
        
        color++;
        delay(2000);
    }
}