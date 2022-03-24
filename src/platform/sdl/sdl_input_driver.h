/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef sense_sdl_input_manager_h
#define sense_sdl_input_manager_h

//#include "sdl_gamepad.h"
#include "sdl_keyboard.h"
//#include "sdl_touches.h"
#include <basal/basal_size2i.h>

struct sense_input;

typedef struct sense_sdl_input_manager {
	bl_size2i screen_size;
	sense_sdl_gamepad gamepad;
	sense_sdl_keys keyboard;
	sense_sdl_touches touches;
} sense_sdl_input_manager;

void sense_sdl_input_manager_init(sense_sdl_input_manager* self, bl_size2i screen_size);
void sense_sdl_input_manager_update(sense_sdl_input_manager* self, struct sense_input* input);

#endif
