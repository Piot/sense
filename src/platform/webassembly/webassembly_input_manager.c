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
#include "webassembly_input_manager.h"
#include <sense/sense_input.h>
#include <sense/sense_input_manager.h>

#include <clog/clog.h>
#include <tiny-libc/tiny_libc.h>

static tyran_boolean key_is_pressed(const sense_keyboard_keys* keys)
{
	return (keys->up != 0 || keys->down != 0 || keys->left != 0 || keys->right != 0 || keys->a != 0 || keys->b != 0 || keys->x != 0 || keys->y != 0);
}

static void update(sense_webassembly_input_manager* self, sense_input* input)
{
	sense_webassembly_gamepad_poll(&self->gamepad);
	if (key_is_pressed(&self->gamepad.keys)) {
		input->keys = self->gamepad.keys;
	} else {
		input->keys = self->keyboard.keys;
	}
	input->touches = self->touches.touches;
	sense_webassembly_touches_clear(&self->touches);
}

void sense_webassembly_input_manager_init(sense_webassembly_input_manager* self, bl_size2i screen_size)
{
	const char* node = "#canvas";

	sense_webassembly_gamepad_init(&self->gamepad);
	sense_webassembly_keys_init(&self->keyboard, node);
	sense_webassembly_touches_init(&self->touches, node, screen_size);
}

void sense_webassembly_input_manager_update(sense_webassembly_input_manager* self, sense_input* input)
{
	update(self, input);
}
