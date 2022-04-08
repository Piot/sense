/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include "glfw_keyboard.h"

#include <clog/clog.h>

#define SENSE_BUTTON_VALUE_MAX (1000)

static GlfwKeyboard* g_glfwKeyboard;

static int convertKeyToGamepadAndButtonIndex(int key, int valueToSet, SenseNamedButtons buttons[4]) {
    int gamepadIndex = -1;
    switch (key) {
        case GLFW_KEY_W:
        case GLFW_KEY_UP:
            g_glfwKeyboard->player1Left.vertical.positiveValue = valueToSet;
            gamepadIndex = 0;
            break;
        case GLFW_KEY_S:
        case GLFW_KEY_DOWN:
            g_glfwKeyboard->player1Left.vertical.negativeValue = valueToSet;
            gamepadIndex = 0;
            break;
        case GLFW_KEY_A:
        case GLFW_KEY_LEFT:
            g_glfwKeyboard->player1Left.horizontal.negativeValue = valueToSet;
            gamepadIndex = 0;
            break;
        case GLFW_KEY_D:
        case GLFW_KEY_RIGHT:
            g_glfwKeyboard->player1Left.horizontal.positiveValue = valueToSet;
            gamepadIndex = 0;
            break;
        case GLFW_KEY_SPACE:
        case GLFW_KEY_ENTER:
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
        case GLFW_KEY_F:
            buttons[0].rightShoulder = valueToSet;
            gamepadIndex = 0;
            break;
        case GLFW_KEY_ESCAPE:
            buttons[0].menu = valueToSet;
            gamepadIndex = 0;
            break;
        case GLFW_KEY_I:
            g_glfwKeyboard->player2Left.vertical.positiveValue = valueToSet;
            gamepadIndex = 1;
            break;
        case GLFW_KEY_K:
            g_glfwKeyboard->player2Left.vertical.negativeValue = valueToSet;
            gamepadIndex = 1;
            break;
        case GLFW_KEY_J:
            g_glfwKeyboard->player2Left.horizontal.negativeValue = valueToSet;
            gamepadIndex = 1;
            break;
        case GLFW_KEY_L:
            g_glfwKeyboard->player2Left.horizontal.positiveValue = valueToSet;
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

    int gamepadIndex = convertKeyToGamepadAndButtonIndex(key, valueToSet, &self->keyboard->named);
    if (gamepadIndex < 0) {
        return;
    }

    if (gamepadIndex == 0) {
        self->keyboard[0].named.horizontal = self->player1Left.horizontal.positiveValue -  self->player1Left.horizontal.negativeValue;
        self->keyboard[0].named.vertical = self->player1Left.vertical.positiveValue -  self->player1Left.vertical.negativeValue;
    } else if (gamepadIndex == 1) {
        self->keyboard[1].named.horizontal = self->player2Left.horizontal.positiveValue -  self->player2Left.horizontal.negativeValue;
        self->keyboard[1].named.vertical = self->player2Left.vertical.positiveValue -  self->player2Left.vertical.negativeValue;
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
