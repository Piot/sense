/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include "webassembly_touches.h"
#include <emscripten/html5.h>

#include <clog/clog.h>
#include <sense/SenseInput.h>
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
		CLOG_OUTPUT("On Touch %ld x %ld", point->clientX, point->clientY);
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
    (void) eventType;
    
	return on_touch((sense_webassembly_touches*) userData, touchEvent);
}

static EM_BOOL on_touch_move(int eventType, const EmscriptenTouchEvent* touchEvent, void* userData)
{
    (void) eventType;

    return on_touch((sense_webassembly_touches*) userData, touchEvent);
}

static EM_BOOL on_touch_end(int eventType, const EmscriptenTouchEvent* touchEvent, void* userData)
{
    (void) eventType;

    return on_touch((sense_webassembly_touches*) userData, touchEvent);
}

static EM_BOOL on_touch_cancel(int eventType, const EmscriptenTouchEvent* touchEvent, void* userData)
{
    (void) eventType;

    return on_touch((sense_webassembly_touches*) userData, touchEvent);
}

static EM_BOOL on_mouse_down(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData)
{
    (void) eventType;

    return on_mouse((sense_webassembly_touches*) userData, mouseEvent);
}

static EM_BOOL on_mouse_up(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData)
{
    (void) eventType;

    return on_mouse((sense_webassembly_touches*) userData, mouseEvent);
}

static EM_BOOL on_mouse_move(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData)
{
    (void) eventType;

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
