#include "input.h"

// Hardware I/O Ports
// 0xA10003 = DATA Port 1 (Controller 1)
// 0xA10009 = CONTROL Port 1 (Direction)
#define PAD1_DATA    (*(vu8 *)0xA10003)
#define PAD1_CTRL    (*(vu8 *)0xA10009)

void input_init() {
    // Set the Control Port for Pad 1
    // We need to set Bit 6 (0x40) to OUTPUT mode. 
    // This pin controls the "Select" line (TH).
    // 0x40 = 0100 0000
    PAD1_CTRL = 0x40;
}

u16 input_read() {
    u8 state_hi, state_lo;
    u16 result = 0;

    // PHASE 1: Write 1 to TH (Select Pin)
    // This allows us to read: CB | RC | R | L | D | U
    PAD1_DATA = 0x40;
    __asm__("nop"); // Small delay to let electricity settle
    __asm__("nop");
    state_hi = PAD1_DATA;

    // PHASE 2: Write 0 to TH (Select Pin)
    // This allows us to read: 0 | A | 0 | 0 | D | U
    PAD1_DATA = 0x00;
    __asm__("nop");
    __asm__("nop");
    state_lo = PAD1_DATA;

    // The Genesis hardware returns 0 for PRESSED and 1 for RELEASED (Active Low).
    // We usually invert this logic in software so 1 = PRESSED for easier coding.
    
    // Combine the bits into a single standard format
    
    // Upper byte (Up, Down, Left, Right) - found in both phases, usually take from hi
    if ((state_hi & 0x01) == 0) result |= BUTTON_UP;
    if ((state_hi & 0x02) == 0) result |= BUTTON_DOWN;
    if ((state_hi & 0x04) == 0) result |= BUTTON_LEFT;
    if ((state_hi & 0x08) == 0) result |= BUTTON_RIGHT;

    // Buttons found in Phase 1 (B, C)
    if ((state_hi & 0x10) == 0) result |= BUTTON_B;
    if ((state_hi & 0x20) == 0) result |= BUTTON_C;

    // Buttons found in Phase 2 (A, Start)
    if ((state_lo & 0x10) == 0) result |= BUTTON_A;
    if ((state_lo & 0x20) == 0) result |= BUTTON_START;

    return result;
}