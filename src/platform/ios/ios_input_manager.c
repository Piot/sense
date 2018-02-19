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
#include "ios_input_manager.h"
#include <sense/sense_input.h>
#include <sense/sense_input_manager.h>

#include <tyran/tyran_clib.h>
#include <tyran/tyran_log.h>

void sense_ios_input_gamepad_manager_update(sense_input* input);

static void update(sense_ios_input_manager* self, sense_input* input)
{
	tyran_mem_clear_type(input);
	sense_ios_input_gamepad_manager_update(input);
	input->touches = self->touches;
}

void sense_ios_input_manager_init(sense_ios_input_manager* self, nimbus_size2i screen_size)
{
	self->screen_size = screen_size;
	sense_touches_init(&self->touches);
}

void sense_ios_input_manager_touch(sense_ios_input_manager* self, size_t id, sense_touch_phase phase, nimbus_vector2i position)
{
	sense_touches_add(&self->touches, id, phase, position);
}

void sense_ios_input_manager_update(sense_ios_input_manager* self, sense_input* input)
{
	update(self, input);
	sense_touches_reset(&self->touches);
}
