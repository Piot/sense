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
#ifndef sense_touches_h
#define sense_touches_h

#include <nimbus/nimbus_vector2.h>
#include <tyran/mutex/nimbus_mutex.h>

typedef enum sense_touch_phase {
	sense_touch_phase_began,
	sense_touch_phase_moved,
	sense_touch_phase_ended,
	sense_touch_phase_cancelled,
} sense_touch_phase;

typedef struct sense_touch {
	size_t identifier;
	nimbus_vector2i position;
	enum sense_touch_phase phase;
} sense_touch;

typedef struct sense_touches {
	nimbus_mutex mutex;
	sense_touch touches[64];
	size_t touch_count;
	size_t max_touch_count;
} sense_touches;

void sense_touches_init(sense_touches* self);

void sense_touches_add(sense_touches* self, size_t identifier, sense_touch_phase phase, nimbus_vector2i position);

void sense_touches_reset(sense_touches* self);

const char* sense_touch_phase_name(sense_touch_phase phase);

#endif
