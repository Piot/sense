/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include "glfw_input_manager.h"
#include <sense/sense_input.h>
#include <sense/sense_input_manager.h>

#include <tiny-libc/tiny_libc.h>

#include <breathe/breathe_app.h> // to get GLFWWindow

static void update(void* _self, SenseInput* target)
{
	SenseGlfwInputManager* self = (SenseGlfwInputManager*) _self;
	senseGlfwInputManagerUpdate(self, target);
}

void senseInputManagerCreatePlatformDriver(SenseInputManager* target, bl_size2i screen_size)
{
	SenseGlfwInputManager* self = tc_malloc_type(SenseGlfwInputManager);
	senseGlfwInputManagerInit(self, g_main->window,  screen_size);

	target->self = self;
	target->update_fn = update;
}

