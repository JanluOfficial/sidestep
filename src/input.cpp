#include "input.h"
#include <raylib.h>

KeyboardKey keyboard_left = KEY_G;
KeyboardKey keyboard_right = KEY_H;

GamepadButton gamepad_left = GAMEPAD_BUTTON_LEFT_TRIGGER_1;
GamepadButton gamepad_right = GAMEPAD_BUTTON_RIGHT_TRIGGER_1;

bool IsInputLeftPressed() {
  return IsKeyPressed(keyboard_left) || IsGamepadButtonPressed(0, gamepad_left);
}

bool IsInputRightPressed() {
  return IsKeyPressed(keyboard_right) || IsGamepadButtonPressed(0, gamepad_right);
}
