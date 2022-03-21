/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef sense_glfw_input_manager_h
#define sense_glfw_input_manager_h

#include "glfw_keyboard.h"
#include <basal/basal_size2i.h>
#include <stdbool.h>

struct SenseInput;

typedef struct GamepadState {
	bool isBound;
} GamepadState;

typedef struct BoundGamepad {
	int joystickId;
    bool isUsed;
} BoundGamepad;

typedef struct SenseGlfwInputManager {
	GamepadState gamepadStates[16];
	BoundGamepad boundGamepads[16];
	int boundGamepadsMax;
	int boundGamepadsCount;

	GlfwKeyboard keyboards;
} SenseGlfwInputManager;

void senseGlfwInputManagerInit(SenseGlfwInputManager* self, GLFWwindow* window, bl_size2i screen_size);
void senseGlfwInputManagerUpdate(SenseGlfwInputManager* self, struct SenseInput* input);

#endif
