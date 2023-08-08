/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef sense_input_manager_h
#define sense_input_manager_h

#include <basal/size2i.h>
#include <basal/vector2i.h>
#include <sense/sense_touches.h>

struct SenseInput;
struct ImprintAllocator;

typedef void (*sense_input_update_fn)(void* driver, struct SenseInput* input);
typedef void (*sense_input_touch_fn)(void* driver, size_t id, sense_touch_phase phase, BlVector2i position);

typedef struct SenseInputManager {
	void* self;
	sense_input_update_fn update_fn;
	sense_input_touch_fn touch_fn;
} SenseInputManager;

void senseInputManagerCreatePlatformDriver(SenseInputManager* self, struct ImprintAllocator* allocator, BlSize2i screen_size);
void senseInputManagerUpdate(SenseInputManager* self, struct SenseInput* input);
void senseInputManagerTouch(SenseInputManager* self, size_t id, sense_touch_phase phase, BlVector2i position);

#endif
