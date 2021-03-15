/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef sense_ios_input_manager_h
#define sense_ios_input_manager_h

#include <basal/basal_size2i.h>
#include <sense/sense_touches.h>
#include <tyran/tyran_types.h>

struct sense_input;

typedef struct sense_ios_input_manager {
	bl_size2i screen_size;
	sense_touches touches;
} sense_ios_input_manager;

void sense_ios_input_manager_init(sense_ios_input_manager* self, bl_size2i screen_size);
void sense_ios_input_manager_update(sense_ios_input_manager* self, struct sense_input* input);
void sense_ios_input_manager_touch(sense_ios_input_manager* self, size_t id, sense_touch_phase phase, bl_vector2i position);

#endif
