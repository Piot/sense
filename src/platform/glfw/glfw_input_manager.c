/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include "glfw_input_manager.h"
#include <sense/sense_input.h>

#include <GLFW/glfw3.h>


#include <clog/clog.h>

static bool key_is_pressed(const SenseNamedButtons *keys)
{
	return (keys->vertical != 0 || keys->horizontal != 0 ||  keys->a != 0 || keys->b != 0 || keys->x != 0 || keys->y != 0);
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

		knownState->isBound = true;
		CLOG_EXECUTE(const char* name = glfwGetGamepadName(joystickId);)
		CLOG_INFO("detected and bound gamepad %d %s", joystickId, name)
		if (self->boundGamepadsCount >= self->boundGamepadsMax) {
			CLOG_ERROR("too many gamepads")
			return;
		}
		BoundGamepad* boundGamepad = &self->boundGamepads[self->boundGamepadsCount++];
		boundGamepad->joystickId = joystickId;
		boundGamepad->isUsed = true;
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

		struct SenseNamedButtons* target = &gamepadStates[i].named;
		unsigned char* source = currentState.buttons;

		const float* sourceAxes = currentState.axes;



		target->a = source[GLFW_GAMEPAD_BUTTON_A] == GLFW_TRUE;
		target->b = source[GLFW_GAMEPAD_BUTTON_B] == GLFW_TRUE;
		target->x = source[GLFW_GAMEPAD_BUTTON_X] == GLFW_TRUE;
		target->y = source[GLFW_GAMEPAD_BUTTON_Y] == GLFW_TRUE;

		target->menu = source[GLFW_GAMEPAD_BUTTON_START] == GLFW_TRUE;

		target->vertical = source[GLFW_GAMEPAD_BUTTON_DPAD_UP] == GLFW_TRUE ? 1000 : 0;
		if (sourceAxes[GLFW_GAMEPAD_AXIS_LEFT_Y] < -0.01f) {
			target->vertical = sourceAxes[GLFW_GAMEPAD_AXIS_LEFT_Y] * 1000;
		}

		target->vertical = source[GLFW_GAMEPAD_BUTTON_DPAD_DOWN] == GLFW_TRUE ? -1000 : 0;
		if (sourceAxes[GLFW_GAMEPAD_AXIS_LEFT_Y] > 0.01f) {
            target->vertical = sourceAxes[GLFW_GAMEPAD_AXIS_LEFT_Y] * 1000;
		}

		target->horizontal = source[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] == GLFW_TRUE ? 1000 : 0;
		if (sourceAxes[GLFW_GAMEPAD_AXIS_LEFT_X] > 0.01f) {
			target->horizontal = sourceAxes[GLFW_GAMEPAD_AXIS_LEFT_X] * 1000;
		}

        target->horizontal = source[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] == GLFW_TRUE ? -1000 : 0;
        if (sourceAxes[GLFW_GAMEPAD_AXIS_LEFT_X] < -0.01f) {
            target->horizontal = sourceAxes[GLFW_GAMEPAD_AXIS_LEFT_X] * 1000;
        }

	}
}

void senseGlfwInputManagerInit(SenseGlfwInputManager*self, GLFWwindow* window, bl_size2i screen_size)
{
	tc_mem_clear_type(self);
	glfwKeyboardInit(&self->keyboards, window);

	self->boundGamepadsMax = 16;
}

void senseGlfwInputManagerUpdate(SenseGlfwInputManager*self, SenseInput*input)
{
	tc_mem_clear_type(input);

	checkForNewGamepads(self);
	scanGamepads(self, &input->devices[2]);
	glfwKeyboardUpdate(&self->keyboards, input->devices, 1);
}
