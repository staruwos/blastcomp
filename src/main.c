#include "types.h"
#include "input.h"

// VDP Ports
#define VDP_DATA    (*(vu16 *)0xC00000)
#define VDP_CTRL    (*(vu32 *)0xC00004)

const u8 vdp_init_values[] = {
    0x04, 0x74, 0x30, 0x00, 0x07, 0x78, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x00, 0x81, 0x3F, 0x00, 0x02,
    0x01, 0x00, 0x00
};

void vdp_init() {
    int i;
    for(i = 0; i < 19; i++) {
        u16 cmd = 0x8000 | (i << 8) | vdp_init_values[i];
        VDP_CTRL = cmd;
    }
}

void main() {
    u16 current_color = 0x0000; // Black

    vdp_init();
    input_init();

    while(1) {
        // 1. Read Input
        u16 joy = input_read();

        // 2. Logic: Change color based on buttons
        if (joy & BUTTON_UP)    current_color = 0x000E; // Red
        if (joy & BUTTON_DOWN)  current_color = 0x00E0; // Green
        if (joy & BUTTON_LEFT)  current_color = 0x0E00; // Blue
        if (joy & BUTTON_RIGHT) current_color = 0x0EE0; // Yellow
        if (joy & BUTTON_A)     current_color = 0x0EEE; // White
        if (joy & BUTTON_START) current_color = 0x0000; // Black (Reset)

        // 3. Render
        // Set write target to CRAM Index 0
        VDP_CTRL = 0xC0000000;
        VDP_DATA = current_color;

        // 4. Small delay to prevent flickering too fast
        // In a real game, you would wait for V-Blank here
        volatile int i;
        for(i=0; i<500; i++) __asm__("nop");
    }
}