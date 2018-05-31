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
#include "webassembly_touches.h"
#include <emscripten/html5.h>

#include <clog/clog.h>
#include <sense/sense_input.h>
#include <sense/sense_input_manager.h>
#include <tiny-libc/tiny_libc.h>
#include <tyran/tyran_types.h>

static int invert_y(const sense_webassembly_touches* self, int y)
{
	return self->screen_size.height - y;
}

static EM_BOOL on_touch(sense_webassembly_touches* self, const EmscriptenTouchEvent* touchEvent)
{
	sense_touches* touches = &self->touches;

	for (int i = 0; i < touchEvent->numTouches; ++i) {
		const EmscriptenTouchPoint* point = &touchEvent->touches[i];
		CLOG_OUTPUT("On Touch %ux%u", point->clientX, point->clientY);
		bl_vector2i pos;
		pos.x = point->clientX;
		pos.y = point->clientY;
		pos.y = invert_y(self, pos.y);

		sense_touches_add(touches, point->identifier, sense_touch_phase_moved, pos);
	}
	return EM_TRUE;
}

static EM_BOOL on_mouse(sense_webassembly_touches* self, const EmscriptenMouseEvent* mouseEvent)
{
	// CLOG_OUTPUT("On Mouse %ux%u", mouseEvent->clientX, mouseEvent->clientY);
	sense_touches* touches = &self->touches;

	bl_vector2i pos;

	pos.x = mouseEvent->clientX;
	pos.y = mouseEvent->clientY;
	pos.y = invert_y(self, pos.y);
	sense_touches_add(touches, -1, sense_touch_phase_moved, pos);
	return EM_TRUE;
}

static EM_BOOL on_touch_start(int eventType, const EmscriptenTouchEvent* touchEvent, void* userData)
{
	return on_touch((sense_webassembly_touches*) userData, touchEvent);
}

static EM_BOOL on_touch_move(int eventType, const EmscriptenTouchEvent* touchEvent, void* userData)
{
	return on_touch((sense_webassembly_touches*) userData, touchEvent);
}

static EM_BOOL on_touch_end(int eventType, const EmscriptenTouchEvent* touchEvent, void* userData)
{
	return on_touch((sense_webassembly_touches*) userData, touchEvent);
}

static EM_BOOL on_touch_cancel(int eventType, const EmscriptenTouchEvent* touchEvent, void* userData)
{
	return on_touch((sense_webassembly_touches*) userData, touchEvent);
}

static EM_BOOL on_mouse_down(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData)
{
	return on_mouse((sense_webassembly_touches*) userData, mouseEvent);
}

static EM_BOOL on_mouse_up(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData)
{
	return on_mouse((sense_webassembly_touches*) userData, mouseEvent);
}

static EM_BOOL on_mouse_move(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData)
{
	if (!mouseEvent->buttons) {
		return EM_FALSE;
	}
	return on_mouse((sense_webassembly_touches*) userData, mouseEvent);
}

void sense_webassembly_touches_clear(sense_webassembly_touches* self)
{
	sense_touches_reset(&self->touches);
}

void sense_webassembly_touches_init(sense_webassembly_touches* self, const char* node, bl_size2i screen_size)
{
	self->screen_size = screen_size;
	sense_touches_init(&self->touches);
	emscripten_set_touchstart_callback(node, self, 0, on_touch_start);
	emscripten_set_touchend_callback(node, self, 0, on_touch_end);
	emscripten_set_touchmove_callback(node, self, 0, on_touch_move);
	emscripten_set_touchcancel_callback(node, self, 0, on_touch_cancel);

	emscripten_set_mousedown_callback(node, self, 0, on_mouse_down);
	emscripten_set_mouseup_callback(node, self, 0, on_mouse_up);
	emscripten_set_mousemove_callback(node, self, 0, on_mouse_move);
}
