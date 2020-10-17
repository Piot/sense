#ifndef SENSE_GLFW_KEYBOARD_H
#define SENSE_GLFW_KEYBOARD_H

#include <sense/sense_buttons.h>
#include <GLFW/glfw3.h>

typedef struct GlfwKeyboard{
	SenseButtons keyboard[4];
	GLFWwindow* window;
} GlfwKeyboard;

void glfwKeyboardInit(GlfwKeyboard* self, GLFWwindow* window);
size_t glfwKeyboardUpdate(GlfwKeyboard* self, SenseButtons* inputs, size_t maxInputs);

#endif
