#ifndef INPUT_H
#define INPUT_H

#include "types.h"

// Controller Buttons (Bitmask)
// These values correspond to the bits we will arrange in our read function
#define BUTTON_UP       0X0001
#define BUTTON_DOWN     0x0002
#define BUTTON_LEFT     0x0004
#define BUTTON_RIGHT    0x0008
#define BUTTON_B        0x0010
#define BUTTON_C        0x0020
#define BUTTON_A        0x0040
#define BUTTON_START    0x0080

// Initialize the controller ports
void input_init();

// Read the current state of Controller 1
// Return: A u16 where 1 = Pressed
u16 input_read();

#endif