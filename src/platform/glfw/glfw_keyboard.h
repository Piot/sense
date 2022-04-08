/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef SENSE_GLFW_KEYBOARD_H
#define SENSE_GLFW_KEYBOARD_H

#include <GLFW/glfw3.h>
#include <sense/sense_buttons.h>

typedef struct GlfwKeyboardAxis {
    int positiveValue;
    int negativeValue;
} GlfwKeyboardAxis;

typedef struct GlfwKeyboardStick {
    GlfwKeyboardAxis horizontal;
    GlfwKeyboardAxis vertical;
} GlfwKeyboardStick;

typedef struct GlfwKeyboard {
	SenseButtons keyboard[4];
    int boundToSourceIndex[4];
    int isIndexBound[4];

    GlfwKeyboardStick player1Left;
    GlfwKeyboardStick player2Left;
    size_t boundedCount;
	GLFWwindow* window;
    GLFWkeyfun previousCallback;
} GlfwKeyboard;

void glfwKeyboardInit(GlfwKeyboard* self, GLFWwindow* window);
size_t glfwKeyboardUpdate(GlfwKeyboard* self, SenseButtons* inputs, size_t maxInputs);

#endif
