/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include "glfw_input_manager.h"
#include <sense/sense_input.h>
#include <sense/sense_input_manager.h>
#include <breathe/breathe_app.h> // to get GLFWWindow
#include <imprint/allocator.h>

static void update(void* _self, SenseInput* target)
{
	SenseGlfwInputManager* self = (SenseGlfwInputManager*) _self;
	senseGlfwInputManagerUpdate(self, target);
}

void senseInputManagerCreatePlatformDriver(SenseInputManager* target, struct ImprintAllocator* allocator, bl_size2i screen_size)
{
	SenseGlfwInputManager* self = IMPRINT_ALLOC_TYPE(allocator, SenseGlfwInputManager);
	senseGlfwInputManagerInit(self, g_main->window,  screen_size);

	target->self = self;
	target->update_fn = update;
}

