/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef sense_webassembly_input_manager_h
#define sense_webassembly_input_manager_h

#include "webassembly_gamepad.h"
#include "webassembly_keyboard.h"
#include "webassembly_touches.h"
#include <basal/basal_size2i.h>
#include <tyran/tyran_types.h>

struct sense_input;

typedef struct sense_webassembly_input_manager {
	bl_size2i screen_size;
	sense_webassembly_gamepad gamepad;
	sense_webassembly_keys keyboard;
	sense_webassembly_touches touches;
} sense_webassembly_input_manager;

void sense_webassembly_input_manager_init(sense_webassembly_input_manager* self, bl_size2i screen_size);
void sense_webassembly_input_manager_update(sense_webassembly_input_manager* self, struct sense_input* input);

#endif
