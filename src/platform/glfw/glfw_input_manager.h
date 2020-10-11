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

#include <basal/basal_size2i.h>
#include <tyran/tyran_types.h>

struct sense_input;

typedef struct GamepadState {
	tyran_boolean isBound;
} GamepadState;

typedef struct BoundGamepad {
	int joystickId;
	tyran_boolean isUsed;
} BoundGamepad;

typedef struct sense_glfw_input_manager {
	bl_size2i screen_size;
	GamepadState gamepadStates[16];
	BoundGamepad boundGamepads[16];
	int boundGamepadsMax;
	int boundGamepadsCount;
} sense_glfw_input_manager;

void sense_glfw_input_manager_init(sense_glfw_input_manager* self, bl_size2i screen_size);
void sense_glfw_input_manager_update(sense_glfw_input_manager* self, struct sense_input* input);

#endif
