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
#ifndef sense_input_manager_h
#define sense_input_manager_h

#include <basal/basal_size2i.h>
#include <sense/sense_touches.h>

struct sense_input;

typedef void (*sense_input_update_fn)(void* driver, struct sense_input* input);
typedef void (*sense_input_touch_fn)(void* driver, size_t id, sense_touch_phase phase, bl_vector2i position);

typedef struct sense_input_driver {
	void* self;
	sense_input_update_fn update_fn;
	sense_input_touch_fn touch_fn;
} sense_input_driver;

void sense_input_manager_create_platform_driver(sense_input_driver* self, bl_size2i screen_size);
void sense_input_manager_update(sense_input_driver* self, struct sense_input* input);
void sense_input_manager_touch(sense_input_driver* self, size_t id, sense_touch_phase phase, bl_vector2i position);

#endif
