/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include "glfw_keyboard.h"

#include <clog/clog.h>

static GlfwKeyboard* g_glfwKeyboard;

int convertKeyToGamepadAndButtonIndex(int key, int* gamepadIndex, SenseButtonNames *buttonIndex) {
    switch (key) {
        case GLFW_KEY_W:
            *gamepadIndex = 0;
            *buttonIndex = SENSE_KEY_UP;
            break;
        case GLFW_KEY_S:
            *gamepadIndex = 0;
            *buttonIndex = SENSE_KEY_DOWN;
            break;
        case GLFW_KEY_A:
            *gamepadIndex = 0;
            *buttonIndex = SENSE_KEY_LEFT;
            break;
        case GLFW_KEY_D:
            *gamepadIndex = 0;
            *buttonIndex = SENSE_KEY_RIGHT;
            break;
        case GLFW_KEY_SPACE:
            *gamepadIndex = 0;
            *buttonIndex = SENSE_KEY_A;
            break;
        case GLFW_KEY_LEFT_SHIFT:
            *gamepadIndex = 0;
            *buttonIndex = SENSE_KEY_B;
            break;
        case GLFW_KEY_Q:
            *gamepadIndex = 0;
            *buttonIndex = SENSE_KEY_X;
            break;
        case GLFW_KEY_E:
            *gamepadIndex = 0;
            *buttonIndex = SENSE_KEY_Y;
            break;

        case GLFW_KEY_I:
            *gamepadIndex = 1;
            *buttonIndex = SENSE_KEY_UP;
            break;
        case GLFW_KEY_K:
            *gamepadIndex = 1;
            *buttonIndex = SENSE_KEY_DOWN;
            break;
        case GLFW_KEY_J:
            *gamepadIndex = 1;
            *buttonIndex = SENSE_KEY_LEFT;
            break;
        case GLFW_KEY_L:
            *gamepadIndex = 1;
            *buttonIndex = SENSE_KEY_RIGHT;
            break;

        case GLFW_KEY_M:
            *gamepadIndex = 1;
            *buttonIndex = SENSE_KEY_A;
            break;
        default:
            return -1;
    }

    return 0;
}


static void onKey(GLFWwindow* window, int key, int scanCode, int action, int bitFieldMods) {
    if (action != GLFW_PRESS && action != GLFW_RELEASE) {
        return;
    }

    int wasPressed = action == GLFW_PRESS;
    int gamepadIndex;
    enum SenseButtonNames buttonIndex;
    int success = convertKeyToGamepadAndButtonIndex(key, &gamepadIndex, &buttonIndex);
    if (success < 0) {
        return;
    }

    GlfwKeyboard* self = g_glfwKeyboard;
    if (wasPressed && !self->isIndexBound[gamepadIndex]) {
        if (self->boundedCount == 4) {
            CLOG_ERROR("out of bounded keyboards")
            return;
        }
        int boundIndex = self->boundedCount++;
        self->boundToSourceIndex[boundIndex] = gamepadIndex;
        self->isIndexBound[gamepadIndex] = 1;
    }

    SenseButtons* buttons = &g_glfwKeyboard->keyboard[gamepadIndex];
    buttons->values[buttonIndex] = wasPressed;

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
