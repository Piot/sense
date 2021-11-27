/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include "glfw_keyboard.h"

#include <clog/clog.h>



static GlfwKeyboard* g_glfwKeyboard;


#define SENSE_BUTTON_VALUE_MAX (1000)




int convertKeyToGamepadAndButtonIndex(int key, int valueToSet, SenseNamedButtons buttons[4]) {
    int gamepadIndex = -1;
    switch (key) {
        case GLFW_KEY_W:
            buttons[0].vertical = valueToSet;
            gamepadIndex = 0;
            break;
        case GLFW_KEY_S:
            buttons[0].vertical = -valueToSet;
            gamepadIndex = 0;
            break;
        case GLFW_KEY_A:
            buttons[0].horizontal = -valueToSet;
            gamepadIndex = 0;
            break;
        case GLFW_KEY_D:
            buttons[0].horizontal = valueToSet;
            gamepadIndex = 0;
            break;
        case GLFW_KEY_SPACE:
            buttons[0].a = valueToSet;
            gamepadIndex = 0;
            break;
        case GLFW_KEY_LEFT_SHIFT:
            buttons[0].b = valueToSet;
            gamepadIndex = 0;
            break;
        case GLFW_KEY_Q:
            buttons[0].x = valueToSet;
            gamepadIndex = 0;
            break;
        case GLFW_KEY_E:
            buttons[0].y = valueToSet;
            gamepadIndex = 0;
            break;
        case GLFW_KEY_I:
            buttons[1].vertical = valueToSet;
            gamepadIndex = 1;
            break;
        case GLFW_KEY_K:
            buttons[1].vertical = -valueToSet;
            gamepadIndex = 1;
            break;
        case GLFW_KEY_J:
            buttons[1].horizontal = -valueToSet;
            gamepadIndex = 1;
            break;
        case GLFW_KEY_L:
            buttons[1].horizontal = valueToSet;
            gamepadIndex = 1;
            break;
        case GLFW_KEY_M:
            buttons[1].a = valueToSet;
            gamepadIndex = 1;
            break;
        default:
            return -1;
    }

    return gamepadIndex;
}


static void onKey(GLFWwindow* window, int key, int scanCode, int action, int bitFieldMods) {
    GlfwKeyboard* self = g_glfwKeyboard;

    self->previousCallback(window, key, scanCode, action, bitFieldMods);

    if (action != GLFW_PRESS && action != GLFW_RELEASE) {
        return;
    }

    int wasPressed = action == GLFW_PRESS;
    int valueToSet = wasPressed ? SENSE_BUTTON_VALUE_MAX : 0;

    int gamepadIndex = convertKeyToGamepadAndButtonIndex(key, valueToSet, self->keyboard);
    if (gamepadIndex < 0) {
        return;
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
    tc_mem_clear_type(&self->keyboard);
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
