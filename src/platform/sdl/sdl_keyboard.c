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
#include "sdl_keyboard.h"
#include "SDL.h"
#include <clog/clog.h>
#include <sense/SenseInput.h>
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
		case 0:
			ref = &k->up;
			break;
		case 1:
			ref = &k->down;
			break;
		case 2:
			ref = &k->left;
			break;
		case 3:
			ref = &k->right;
			break;
		case 4:
			ref = &k->a;
			break;
		case 5:
			ref = &k->b;
			break;
		case 8:
			ref = &k->menu;
			break;
		default:
			return;
	}

	*ref = state ? 1 : 0;
}

static size_t handle_known_keys(int keyname)
{
	int keynames[] = {'w', 's', 'a', 'd', 'q', 'e', 'r', 'f', SDLK_ESCAPE, 'i', 'k', 'j', 'l', 'o', 'u', 'y', 'h', 'n'};

	for (size_t i = 0; i < sizeof(keynames) / sizeof(int); ++i) {
		if (keynames[i] == keyname) {
			return i + 1;
		}
	}
	return 0;
}

static tyran_boolean on_key(sense_sdl_keys* self, const SDL_KeyboardEvent* keyEvent, tyran_boolean state)
{
	if (keyEvent->repeat) {
		return TYRAN_FALSE;
	}
	size_t detected_key = handle_known_keys(keyEvent->keysym.sym);

	if (detected_key) {
		detected_key--;
		size_t player_index = detected_key / 9;
		size_t local_key = detected_key % 9;
		CLOG_OUTPUT("onkey player %zu key %zu on %d", player_index, local_key, state);
		self->device_in_focus = TYRAN_TRUE;
		handle_key(&self->keys[player_index], local_key, state);
		return TYRAN_TRUE;
	}
	return TYRAN_FALSE;
}

static tyran_boolean handle_key_down(sense_sdl_keys* self, const SDL_KeyboardEvent* keyEvent)
{
	return on_key(self, keyEvent, TYRAN_TRUE);
}

static tyran_boolean handle_key_up(sense_sdl_keys* self, const SDL_KeyboardEvent* keyEvent)
{
	return on_key(self, keyEvent, TYRAN_FALSE);
}

int sense_sdl_keys_poll(sense_sdl_keys* self)
{
	SDL_Event event;
	int quit = 0;

	while (SDL_PollEvent(&event)) {

		switch (event.type) {
			case SDL_QUIT:
				quit = 1;
				break;
			case SDL_KEYDOWN:
				handle_key_down(self, &event.key);
				break;
			case SDL_KEYUP:
				handle_key_up(self, &event.key);
				break;
			case SDL_TEXTINPUT:
				// handle_text_input(ipu, &event.text);
				break;
		}
	}

	return quit;
}

void sense_sdl_keys_init(sense_sdl_keys* self)
{
	CLOG_OUTPUT("sdl keys");
	tc_mem_clear_type(self);
}
