#include <WinSock2.h>
#include <windows.h>

#include <Xinput.h>
#include <sense/sense_input.h>
#include <winerror.h>
#include <clog/clog.h>
#include "xinput_manager.h"

static void scanGamepads(SenseXInputManager* self, SenseButtons gamepadStates[8])
{
    XINPUT_STATE state;

    for (size_t i = 0; i < 4; ++i) {
        DWORD status = XInputGetState(i, &state);
        if (status == ERROR_SUCCESS) {
            SenseNamedButtons* button = &gamepadStates[i].named;
            DWORD btn = state.Gamepad.wButtons;
            button->up = btn & XINPUT_GAMEPAD_DPAD_UP;
            button->down = btn & XINPUT_GAMEPAD_DPAD_DOWN;
            button->left = btn & XINPUT_GAMEPAD_DPAD_LEFT;
            button->right = btn & XINPUT_GAMEPAD_DPAD_RIGHT;
            button->a = btn & XINPUT_GAMEPAD_A;
            button->b = btn & XINPUT_GAMEPAD_B;
            button->x = btn & XINPUT_GAMEPAD_X;
            button->y = btn & XINPUT_GAMEPAD_Y;
            button->menu = btn & XINPUT_GAMEPAD_START;

            if (!button->up) {
                button->up = state.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
            }

            if (!button->down) {
                button->down = state.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
            }

            if (!button->left) {
                button->left = state.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
            }

            if (!button->right) {
                button->right = state.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
            }
        }
    }
}

void senseXInputManagerInit(SenseXInputManager* self, bl_size2i screen_size)
{

}

void senseXInputManagerUpdate(SenseXInputManager* self, SenseInput* input)
{
    scanGamepads(self, &input->devices[0]);
}
