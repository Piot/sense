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
#include <emscripten/html5.h>

#include <clog/clog.h>
#include <sense/sense_input.h>
#include <sense/sense_input_manager.h>
#include <tiny-libc/tiny_libc.h>
#include <tyran/tyran_types.h>

#include "webassembly_gamepad.h"

static int gamepad_button(double value, int factor)
{
	const float minimum_value = 0.15f;

	if (value >= -minimum_value && value <= minimum_value) {
		return 0;
	}
	double fv = ((double) factor * value);
	int v = (int) fv;
	if (v < 0) {
		v = 0;
	}

	return v;
}

static const int button_max_value = 100;

void sense_webassembly_gamepad_init(sense_webassembly_gamepad* self)
{
	tc_mem_clear_type(self);
	self->bound_gamepad_index = 0xff;
}

void sense_webassembly_gamepad_poll(sense_webassembly_gamepad* self)
{
	int count = emscripten_get_num_gamepads();

	for (int gamepad_index = 0; gamepad_index < count; ++gamepad_index) {
		EmscriptenGamepadEvent gamepad_status;
		EMSCRIPTEN_RESULT worked = emscripten_get_gamepad_status(gamepad_index, &gamepad_status);
		if (worked != EMSCRIPTEN_RESULT_SUCCESS) {
			continue;
		}
		if (gamepad_status.connected == EM_FALSE) {
			continue;
		}

		if (gamepad_status.numAxes < 2) {
			continue;
		}
		if (gamepad_status.numButtons < 16) {
			continue;
		}
		double menu = 0;
		if (gamepad_status.numButtons > 16) {
			menu = gamepad_status.analogButton[16];
		} else {
			menu = gamepad_status.analogButton[9];
		}

		tyran_boolean bind = gamepad_status.analogButton[4] > 0.1f && gamepad_status.analogButton[5] > 0.1f;

		if (self->bound_gamepad_index == 0xff && (menu > 0.1f || bind)) {
			self->bound_gamepad_index = gamepad_index;
			CLOG_INFO("Bound gamepad to %d", self->bound_gamepad_index);
		}
		if (self->bound_gamepad_index == 0xff) {
			continue;
		}
		double x_axis = gamepad_status.axis[0];
		double y_axis = -gamepad_status.axis[1];
		double dpad_up = gamepad_status.analogButton[12];
		double dpad_down = gamepad_status.analogButton[13];
		double dpad_left = gamepad_status.analogButton[14];
		double dpad_right = gamepad_status.analogButton[15];

		sense_keyboard_keys keys;
		keys.up = gamepad_button(y_axis, button_max_value) | gamepad_button(dpad_up, button_max_value);
		keys.down = gamepad_button(y_axis, -button_max_value) | gamepad_button(dpad_down, button_max_value);
		keys.left = gamepad_button(x_axis, -button_max_value) | gamepad_button(dpad_left, button_max_value);
		keys.right = gamepad_button(x_axis, button_max_value) | gamepad_button(dpad_right, button_max_value);
		keys.a = gamepad_button(gamepad_status.analogButton[0], button_max_value);
		keys.b = gamepad_button(gamepad_status.analogButton[1], button_max_value);
		keys.x = gamepad_button(gamepad_status.analogButton[2], button_max_value);
		keys.y = gamepad_button(gamepad_status.analogButton[3], button_max_value);
		keys.menu = gamepad_button(menu, button_max_value);

		self->keys = keys;
	}
}
