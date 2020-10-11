/*

MIT License

Copyright (c) 2012 Peter Bjorklund

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
#include "glfw_input_manager.h"
#include <sense/sense_input.h>

#include <GLFW/glfw3.h>

#include <clog/clog.h>

static tyran_boolean key_is_pressed(const SenseButtons*keys)
{
	return (keys->up != 0 || keys->down != 0 || keys->left != 0 || keys->right != 0 || keys->a != 0 || keys->b != 0 || keys->x != 0 || keys->y != 0);
}

static void checkForNewGamepads(SenseGlfwInputManager* self)
{
	for (int joystickId = GLFW_JOYSTICK_1; joystickId <= GLFW_JOYSTICK_LAST; ++joystickId)
	{
		if (!glfwJoystickPresent(joystickId)) {
			continue;
		}

		if (!glfwJoystickIsGamepad(joystickId)) {
			continue;
		}

		GamepadState* knownState = &self->gamepadStates[joystickId];
		if (knownState->isBound) {
			continue;
		}

		GLFWgamepadstate currentState;

		int worked = glfwGetGamepadState(joystickId, &currentState);
		if (worked != GLFW_TRUE) {
			continue;
		}

		if (currentState.buttons[GLFW_GAMEPAD_BUTTON_GUIDE] != GLFW_PRESS && currentState.buttons[GLFW_GAMEPAD_BUTTON_START] != GLFW_PRESS) {
			continue;
		}

		knownState->isBound = TYRAN_TRUE;
		const char* name = glfwGetGamepadName(joystickId);
		CLOG_INFO("detected and bound gamepad %d %s", joystickId, name)
		if (self->boundGamepadsCount >= self->boundGamepadsMax) {
			CLOG_ERROR("too many gamepads")
			return;
		}
		BoundGamepad* boundGamepad = &self->boundGamepads[self->boundGamepadsCount++];
		boundGamepad->joystickId = joystickId;
		boundGamepad->isUsed = TYRAN_TRUE;
	}
}

static void scanGamepads(SenseGlfwInputManager* self, SenseButtons gamepadStates[8])
{
	for (int i=0; i<self->boundGamepadsCount; ++i)
	{
		BoundGamepad* gamepad = &self->boundGamepads[i];
		if (!gamepad->isUsed) {
			tc_mem_clear_type(&gamepadStates[i]);
			continue;
		}

		GLFWgamepadstate currentState;
		int worked = glfwGetGamepadState(gamepad->joystickId, &currentState);
		if (worked != GLFW_TRUE) {
			tc_mem_clear_type(&gamepadStates[i]);
			continue;
		}

		SenseButtons* target = &gamepadStates[i];
		unsigned char* source = currentState.buttons;

		target->a = source[GLFW_GAMEPAD_BUTTON_A] == GLFW_TRUE;
		target->b = source[GLFW_GAMEPAD_BUTTON_B] == GLFW_TRUE;
		target->x = source[GLFW_GAMEPAD_BUTTON_X] == GLFW_TRUE;
		target->y = source[GLFW_GAMEPAD_BUTTON_Y] == GLFW_TRUE;

		target->menu = source[GLFW_GAMEPAD_BUTTON_START] == GLFW_TRUE;

		target->up = source[GLFW_GAMEPAD_BUTTON_DPAD_UP] == GLFW_TRUE;
		target->down = source[GLFW_GAMEPAD_BUTTON_DPAD_DOWN] == GLFW_TRUE;
		target->left = source[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] == GLFW_TRUE;
		target->right = source[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] == GLFW_TRUE;
	}
}

void senseGlfwInputManagerInit(SenseGlfwInputManager*self, bl_size2i screen_size)
{
	tc_mem_clear_type(self);

	self->boundGamepadsMax = 16;
}

void senseGlfwInputManagerUpdate(SenseGlfwInputManager*self, SenseInput*input)
{
	tc_mem_clear_type(input);

	checkForNewGamepads(self);
	scanGamepads(self, input->keys);
}
