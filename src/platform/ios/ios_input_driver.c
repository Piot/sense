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
