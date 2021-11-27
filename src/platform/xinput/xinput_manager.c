#include <WinSock2.h>
#include <windows.h>

#include <Xinput.h>
#include <sense/sense_input.h>
#include <winerror.h>
#include <clog/clog.h>
#include "xinput_manager.h"

#define MAX_VALUE (1000)

static void scanGamepads(SenseXInputManager* self, SenseButtons gamepadStates[8])
{
    XINPUT_STATE state;

    for (size_t i = 0; i < 4; ++i) {
        DWORD status = XInputGetState(i, &state);
        if (status == ERROR_SUCCESS) {
            SenseNamedButtons* button = &gamepadStates[i].named;
            DWORD btn = state.Gamepad.wButtons;
            button->vertical = btn & XINPUT_GAMEPAD_DPAD_UP ? MAX_VALUE : ( btn & XINPUT_GAMEPAD_DPAD_DOWN ? -MAX_VALUE : 0);
            button->horizontal = btn & XINPUT_GAMEPAD_DPAD_RIGHT ? MAX_VALUE : ( btn & XINPUT_GAMEPAD_DPAD_LEFT ? -MAX_VALUE : 0);
            button->a = btn & XINPUT_GAMEPAD_A;
            button->b = btn & XINPUT_GAMEPAD_B;
            button->leftShoulder = btn & XINPUT_GAMEPAD_LEFT_SHOULDER;
            button->rightShoulder = btn & XINPUT_GAMEPAD_RIGHT_SHOULDER;
            button->x = btn & XINPUT_GAMEPAD_X;
            button->y = btn & XINPUT_GAMEPAD_Y;
            button->menu = btn & XINPUT_GAMEPAD_START;

            if (button->vertical == 0) {
                if (state.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || state.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
                    button->vertical = state.Gamepad.sThumbLY * MAX_VALUE / 32768;
                }
            }

            if (button->horizontal == 0) {
                if (state.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || state.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
                    button->horizontal = state.Gamepad.sThumbLX * MAX_VALUE / 32768;
                }
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
