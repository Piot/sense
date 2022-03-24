/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include "sdl_keyboard.h"
#include "SDL.h"
#include <clog/clog.h>
#include <sense/SenseInput.h>
#include <sense/sense_input_manager.h>
#include <stdbool.h>
#include <tiny-libc/tiny_libc.h>

static int string_is_equal(const char* a, const char* b)
{
    return strcmp(a, b) == 0;
}

static void handle_key(sense_keyboard_keys* k, int key_down, bool state)
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

static bool on_key(sense_sdl_keys* self, const SDL_KeyboardEvent* keyEvent, bool state)
{
    if (keyEvent->repeat) {
        return false;
    }
    size_t detected_key = handle_known_keys(keyEvent->keysym.sym);

    if (detected_key) {
        detected_key--;
        size_t player_index = detected_key / 9;
        size_t local_key = detected_key % 9;
        CLOG_OUTPUT("onkey player %zu key %zu on %d", player_index, local_key, state);
        self->device_in_focus = true;
        handle_key(&self->keys[player_index], local_key, state);
        return true;
    }
    return false;
}

static bool handle_key_down(sense_sdl_keys* self, const SDL_KeyboardEvent* keyEvent)
{
    return on_key(self, keyEvent, true);
}

static bool handle_key_up(sense_sdl_keys* self, const SDL_KeyboardEvent* keyEvent)
{
    return on_key(self, keyEvent, false);
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
