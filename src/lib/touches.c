/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <sense/sense_touches.h>

void sense_touches_init(sense_touches* self)
{
	self->max_touch_count = 64;
	self->touch_count = 0;

//	latch_mutex_init(&self->mutex);
}

static const char* phase_to_string(sense_touch_phase phase)
{
	switch (phase) {
		case sense_touch_phase_began:
			return "began";
		case sense_touch_phase_cancelled:
			return "cancelled";
		case sense_touch_phase_ended:
			return "ended";
		case sense_touch_phase_moved:
			return "moved";
	}

	return "unknown";
}

const char* sense_touch_phase_name(sense_touch_phase phase)
{
	return phase_to_string(phase);
}

#if defined CLOG_LOG_ENABLED
static void sense_touch_debug(const sense_touch* self)
{
	(void) self;
	CLOG_INFO("[touch id:%zu x:%d y:%d %s]", self->identifier, self->position.x, self->position.y, phase_to_string(self->phase))
}
#endif

void sense_touches_add(sense_touches* self, size_t identifier, sense_touch_phase phase, BlVector2i position)
{
	if (self->touch_count >= self->max_touch_count) {
		CLOG_OUTPUT("Out of touch space! %zu of %zu", self->touch_count, self->max_touch_count)
		return;
	}
	//latch_mutex_lock(&self->mutex);

	sense_touch* touch = &self->touches[self->touch_count++];
	touch->identifier = identifier;
	touch->position = position;
	touch->phase = phase;
	//latch_mutex_unlock(&self->mutex);
}

void sense_touches_reset(sense_touches* self)
{
	//latch_mutex_lock(&self->mutex);
	self->touch_count = 0;

	//latch_mutex_unlock(&self->mutex);
}
