#include "types.h"
#include "input.h"
#include "vdp.h"

void main() {
    u16 current_color = 0x0000; // Black

    // Initialize subsystems
    vdp_init();
    input_init();

    while(1) {
        // 1. Read Input
        u16 joy = input_read();

        // 2. Logic: Change color based on buttons
        // Colors are 0x0BGR format
        if (joy & BUTTON_UP)    current_color = 0x000E; // Red
        if (joy & BUTTON_DOWN)  current_color = 0x00E0; // Green
        if (joy & BUTTON_LEFT)  current_color = 0x0E00; // Blue
        if (joy & BUTTON_RIGHT) current_color = 0x0EE0; // Yellow
        if (joy & BUTTON_A)     current_color = 0x0EEE; // White
        if (joy & BUTTON_START) current_color = 0x0000; // Black (Reset)

        // 3. Render
        // Update Palette 0, Color 0 (Background)
        vdp_set_color(0, current_color);

        // 4. Delay
        volatile int i;
        for(i=0; i<500; i++) __asm__("nop");
    }
}