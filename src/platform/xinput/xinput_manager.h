/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef sense_xinput_manager_h
#define sense_xinput_manager_h

#include <basal/basal_size2i.h>
#include <tyran/tyran_types.h>

struct SenseInput;

typedef struct GamepadState {
    tyran_boolean isBound;
} GamepadState;

typedef struct BoundGamepad {
    int joystickId;
    tyran_boolean isUsed;
} BoundGamepad;

typedef struct SenseXInputManager {
    GamepadState gamepadStates[16];
    BoundGamepad boundGamepads[16];
    int boundGamepadsMax;
    int boundGamepadsCount;

//    XKeyboard keyboards;
} SenseXInputManager;

void senseXInputManagerInit(SenseXInputManager* self, bl_size2i screen_size);
void senseXInputManagerUpdate(SenseXInputManager* self, struct SenseInput* input);

#endif
