/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include "webassembly_input_manager.h"
#include <sense/SenseInput.h>
#include <sense/sense_input_manager.h>

#include <tiny-libc/tiny_libc.h>

static void update(void* _self, sense_input* target)
{
	sense_webassembly_input_manager* self = (sense_webassembly_input_manager*) _self;
	sense_webassembly_input_manager_update(self, target);
}

void senseInputManagerCreatePlatformDriver(SenseInputManager* self, bl_size2i screen_size)
{
	sense_webassembly_input_manager* self = tc_malloc_type(sense_webassembly_input_manager);
	sense_webassembly_input_manager_init(self, screen_size);
	self->self = self;
	self->update_fn = update;
}
