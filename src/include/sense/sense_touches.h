/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef sense_touches_h
#define sense_touches_h

#include <basal/basal_vector2.h>
//#include <latch/mutex.h>
#include <stddef.h>

typedef enum sense_touch_phase {
	sense_touch_phase_began,
	sense_touch_phase_moved,
	sense_touch_phase_ended,
	sense_touch_phase_cancelled,
} sense_touch_phase;

typedef struct sense_touch {
	size_t identifier;
	bl_vector2i position;
	enum sense_touch_phase phase;
} sense_touch;

typedef struct sense_touches {
	//latch_mutex mutex;
	sense_touch touches[64];
	size_t touch_count;
	size_t max_touch_count;
} sense_touches;

void sense_touches_init(sense_touches* self);

void sense_touches_add(sense_touches* self, size_t identifier, sense_touch_phase phase, bl_vector2i position);

void sense_touches_reset(sense_touches* self);

const char* sense_touch_phase_name(sense_touch_phase phase);

#endif
