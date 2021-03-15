/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include "ios_input_manager.h"
#include <sense/SenseInput.h>
#include <sense/sense_input_manager.h>

#include <clog/clog.h>
#include <tiny-libc/tiny_libc.h>

void sense_ios_input_gamepad_manager_update(sense_input* input);

static void update(sense_ios_input_manager* self, sense_input* input)
{
	tc_mem_clear_type(input);
	sense_ios_input_gamepad_manager_update(input);
	input->touches = self->touches;
}

void sense_ios_input_manager_init(sense_ios_input_manager* self, bl_size2i screen_size)
{
	self->screen_size = screen_size;
	sense_touches_init(&self->touches);
}

void sense_ios_input_manager_touch(sense_ios_input_manager* self, size_t id, sense_touch_phase phase, bl_vector2i position)
{
	sense_touches_add(&self->touches, id, phase, position);
}

void sense_ios_input_manager_update(sense_ios_input_manager* self, sense_input* input)
{
	update(self, input);
	sense_touches_reset(&self->touches);
}
