/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include "glfw_keyboard.h"

#include <clog/clog.h>

static GlfwKeyboard* g_glfwKeyboard;

int convertKeyToGamepadAndButtonIndex(int key, int* gamepadIndex, SenseNamedButtons* target) {
    switch (key) {
        case GLFW_KEY_W:
            *gamepadIndex = 0;
            target->vertical = 1000;
            break;
        case GLFW_KEY_S:
            *gamepadIndex = 0;
            target->vertical = -1000;
            break;
        case GLFW_KEY_A:
            *gamepadIndex = 0;
            target->horizontal = -1000;
            break;
        case GLFW_KEY_D:
            *gamepadIndex = 0;
            target->horizontal = 1000;
            break;
        case GLFW_KEY_SPACE:
            *gamepadIndex = 0;
            target->a = 1000;
            break;
        case GLFW_KEY_LEFT_SHIFT:
            *gamepadIndex = 0;
            target->b = 1000;
            break;
        case GLFW_KEY_Q:
            *gamepadIndex = 0;
            target->x = 1000;
            break;
        case GLFW_KEY_E:
            *gamepadIndex = 0;
            target->y = 1000;
            break;

        case GLFW_KEY_I:
            *gamepadIndex = 1;
            target->vertical = 1000;
            break;
        case GLFW_KEY_K:
            *gamepadIndex = 1;
            target->vertical = -1000;
            break;
        case GLFW_KEY_J:
            *gamepadIndex = 1;
            target->horizontal = -1000;
            break;
        case GLFW_KEY_L:
            *gamepadIndex = 1;
            target->horizontal = 1000;
            break;

        case GLFW_KEY_M:
            *gamepadIndex = 1;
            target->a = SENSE_KEY_A;
            break;
        default:
            return -1;
    }

    return 0;
}


static void onKey(GLFWwindow* window, int key, int scanCode, int action, int bitFieldMods) {
    GlfwKeyboard* self = g_glfwKeyboard;

    self->previousCallback(window, key, scanCode, action, bitFieldMods);

    if (action != GLFW_PRESS && action != GLFW_RELEASE) {
        return;
    }

    int wasPressed = action == GLFW_PRESS;
    int gamepadIndex;
    SenseNamedButtons buttons;
    tc_mem_clear_type(&buttons);

    if (wasPressed) {
        int success = convertKeyToGamepadAndButtonIndex(key, &gamepadIndex, &buttons);
        if (success < 0) {
            return;
        }
    }

    if (wasPressed && !self->isIndexBound[gamepadIndex]) {
        if (self->boundedCount == 4) {
            CLOG_ERROR("out of bounded keyboards")
            return;
        }
        int boundIndex = self->boundedCount++;
        self->boundToSourceIndex[boundIndex] = gamepadIndex;
        self->isIndexBound[gamepadIndex] = 1;
    }

    SenseNamedButtons* targetButtons = &g_glfwKeyboard->keyboard[gamepadIndex];
    *targetButtons = buttons;

    self->previousCallback(window, key, scanCode, action, bitFieldMods);
}

void glfwKeyboardInit(GlfwKeyboard* self, GLFWwindow* window)
{
    g_glfwKeyboard = self;
	GLFWkeyfun previousCallback = glfwSetKeyCallback(window, onKey);
	if (!previousCallback) {
		CLOG_ERROR("should always have previous callback");
	}
    self->previousCallback = previousCallback;
	self->window = window;
}


size_t glfwKeyboardUpdate(GlfwKeyboard* self, SenseButtons* inputs, size_t maxInputs)
{
	if (maxInputs == 0) {
		return 0;
	}

    for (size_t i=0; i < self->boundedCount; ++i) {
        int mapping = self->boundToSourceIndex[i];
        inputs[i] = self->keyboard[mapping];
    }

	return self->boundedCount;
}
