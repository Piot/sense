/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include "webassembly_input_manager.h"
#include <sense/SenseInput.h>
#include <sense/sense_input_manager.h>

#include <clog/clog.h>
#include <tiny-libc/tiny_libc.h>

static tyran_boolean key_is_pressed(const sense_keyboard_keys *keys)
{
	return (keys->up != 0 || keys->down != 0 || keys->left != 0 || keys->right != 0 || keys->a != 0 || keys->b != 0 || keys->x != 0 || keys->y != 0);
}

static void update(sense_webassembly_input_manager *self, sense_input *input)
{
	sense_webassembly_gamepad_poll(&self->gamepad);
	if (key_is_pressed(&self->gamepad.keys))
	{
		input->keys[0] = self->gamepad.keys;
	}
	else
	{
		tc_memcpy_type_n(input->keys, self->keyboard.keys, 8);
	}
	input->touches = self->touches.touches;
	sense_webassembly_touches_clear(&self->touches);
}

void sense_webassembly_input_manager_init(sense_webassembly_input_manager *self, bl_size2i screen_size)
{
	const char *node = "#canvas";

	sense_webassembly_gamepad_init(&self->gamepad);
	const char *input_node = 0;
	sense_webassembly_keys_init(&self->keyboard, input_node);
	sense_webassembly_touches_init(&self->touches, node, screen_size);
}

void sense_webassembly_input_manager_update(sense_webassembly_input_manager *self, sense_input *input)
{
	update(self, input);
}
