#include "Vec2.h"

char Input::keys[256] = { 0 };
char Input::oldKeys[256] = { 0 };
int Input::mouseX = 0;
int Input::mouseY = 0;
int Input::oldMouseX = 0;
int Input::oldMouseY = 0;
int Input::mouseBottom = 0;
int Input::oldMouseBottom = 0;
XINPUT_STATE Input::joypad;
XINPUT_STATE Input::oldjoypad;
bool Input::controller = false;