#include "input.h"
#include <raylib.h>

KeyboardKey keyboard_left = KEY_G;
KeyboardKey keyboard_right = KEY_H;

GamepadButton gamepad_left = GAMEPAD_BUTTON_LEFT_TRIGGER_1;
GamepadButton gamepad_right = GAMEPAD_BUTTON_RIGHT_TRIGGER_1;

bool IsActionPressed(KeyboardKey key, GamepadButton button) {
  bool gp = IsGamepadAvailable(0) && IsGamepadButtonPressed(0, button);
  return gp || IsKeyPressed(key);
}

bool IsInputLeftPressed() { return IsActionPressed(keyboard_left, gamepad_left); }
bool IsInputRightPressed() { return IsActionPressed(keyboard_right, gamepad_right); }
