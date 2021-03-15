/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include "ios_input_manager.h"
#include <sense/sense_input.h>
#include <sense/sense_input_manager.h>
#include <tiny-libc/tiny_libc.h>

static void update(void* _self, sense_input* target)
{
	sense_ios_input_manager* self = (sense_ios_input_manager*) _self;
	sense_ios_input_manager_update(self, target);
}

static void touch(void* _self, size_t id, sense_touch_phase phase, bl_vector2i position)
{
	sense_ios_input_manager* self = (sense_ios_input_manager*) _self;
	sense_ios_input_manager_touch(self, id, phase, position);
}

void senseInputManagerCreatePlatformDriver(SenseInputManager* self, bl_size2i screen_size)
{
	sense_ios_input_manager* self = tc_malloc_type(sense_ios_input_manager);
	sense_ios_input_manager_init(self, screen_size);
	self->self = self;
	self->update_fn = update;
	self->touch_fn = touch;
}
