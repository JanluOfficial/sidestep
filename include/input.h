#ifndef INPUT_H
#define INPUT_H

#include <raylib.h>

extern KeyboardKey keyboard_left;
extern KeyboardKey keyboard_right;

extern GamepadButton gamepad_left;
extern GamepadButton gamepad_right;

bool IsInputLeftPressed();
bool IsInputRightPressed();

#endif // INPUT_H
