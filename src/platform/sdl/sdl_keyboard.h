/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef sense_sdl_keys_h
#define sense_sdl_keys_h

#include <sense/sense_keyboard_keys.h>
#include <tyran/tyran_types.h>

typedef struct sense_sdl_keys {
	sense_keyboard_keys keys[8];
	tyran_boolean device_in_focus;
} sense_sdl_keys;

void sense_sdl_keys_init(sense_sdl_keys* self);
int sense_sdl_keys_poll(sense_sdl_keys* self);

#endif
