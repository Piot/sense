/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <sense/sense_input_manager.h>

void senseInputManagerUpdate(SenseInputManager* self, struct SenseInput* input)
{
	self->update_fn(self->self, input);
}

void senseInputManagerTouch(SenseInputManager* self, size_t id, sense_touch_phase phase, BlVector2i position)
{
	self->touch_fn(self->self, id, phase, position);
}
