/*

MIT License

Copyright (c) 2020 Peter Bjorklund

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
#ifndef sense_glfw_input_manager_h
#define sense_glfw_input_manager_h

#include "glfw_keyboard.h"
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
