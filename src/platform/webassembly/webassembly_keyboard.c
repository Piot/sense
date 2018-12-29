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
#include "webassembly_keyboard.h"
#include <emscripten/html5.h>

#include <clog/clog.h>
#include <sense/sense_input.h>
#include <sense/sense_input_manager.h>
#include <tiny-libc/tiny_libc.h>
#include <tyran/tyran_types.h>

static int string_is_equal(const char* a, const char* b)
{
	return strcmp(a, b) == 0;
}

static void handle_key(sense_keyboard_keys* k, int key_down, tyran_boolean state)
{
	int* ref = 0;

	switch (key_down) {
		case 1:
			ref = &k->up;
			break;
		case 2:
			ref = &k->down;
			break;
		case 3:
			ref = &k->left;
			break;
		case 4:
			ref = &k->right;
			break;
		case 5:
			ref = &k->a;
			break;
		case 6:
			ref = &k->b;
			break;
		default:
			return;
	}

	*ref = state ? 1 : 0;
}

static int handle_known_keys(const char* keyname)
{
	if (string_is_equal(keyname, "ArrowUp")) {
		return 1;
	} else if (string_is_equal(keyname, "ArrowDown")) {
		return 2;
	} else if (string_is_equal(keyname, "ArrowLeft")) {
		return 3;
	} else if (string_is_equal(keyname, "ArrowRight")) {
		return 4;
	} else if (string_is_equal(keyname, "z")) {
		return 5;
	} else if (string_is_equal(keyname, "x")) {
		return 6;
	}
	return 0;
}

static EM_BOOL on_key(sense_webassembly_keys* self, const EmscriptenKeyboardEvent* keyEvent, tyran_boolean state)
{
	CLOG_INFO("Detected key! %s", keyEvent->key);
	if (keyEvent->repeat) {
		return EM_FALSE;
	}

	int detected_key = handle_known_keys(keyEvent->key);

	if (detected_key) {
		self->device_in_focus = TYRAN_TRUE;
		handle_key(&self->keys, detected_key, state);
		return EM_TRUE;
	}
	return EM_FALSE;
}

static EM_BOOL on_key_down(int eventType, const EmscriptenKeyboardEvent* keyEvent, void* user_data)
{
    (void) eventType;
    
	return on_key((sense_webassembly_keys*) user_data, keyEvent, TYRAN_TRUE);
}

static EM_BOOL on_key_up(int eventType, const EmscriptenKeyboardEvent* keyEvent, void* user_data)
{
    (void) eventType;
    
	return on_key((sense_webassembly_keys*) user_data, keyEvent, TYRAN_FALSE);
}

void sense_webassembly_keys_init(sense_webassembly_keys* self, const char* node)
{
	CLOG_OUTPUT("Setting callbacks!");
	tc_mem_clear_type(self);
	emscripten_set_keydown_callback(node, self, 1, on_key_down);
	emscripten_set_keyup_callback(node, self, 1, on_key_up);
}
