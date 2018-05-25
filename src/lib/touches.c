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
#include <clog/clog.h>
#include <sense/sense_touches.h>

void sense_touches_init(sense_touches* self)
{
	self->max_touch_count = 64;
	self->touch_count = 0;

	tyran_mutex_init(&self->mutex);
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
}

const char* sense_touch_phase_name(sense_touch_phase phase)
{
	return phase_to_string(phase);
}

void sense_touch_debug(const sense_touch* self)
{
	CLOG_INFO("[touch id:%zu x:%d y:%d %s]", self->identifier, self->position.x, self->position.y, phase_to_string(self->phase));
}

void sense_touches_add(sense_touches* self, size_t identifier, sense_touch_phase phase, bl_vector2i position)
{
	if (self->touch_count >= self->max_touch_count) {
		CLOG_OUTPUT("Out of touch space! %zu of %zu", self->touch_count, self->max_touch_count);
		return;
	}
	tyran_mutex_lock(&self->mutex);

	sense_touch* touch = &self->touches[self->touch_count++];
	touch->identifier = identifier;
	touch->position = position;
	touch->phase = phase;
	tyran_mutex_unlock(&self->mutex);
}

void sense_touches_reset(sense_touches* self)
{
	tyran_mutex_lock(&self->mutex);
	self->touch_count = 0;

	tyran_mutex_unlock(&self->mutex);
}
