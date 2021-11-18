/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include "xinput_manager.h"
#include <sense/sense_input.h>
#include <sense/sense_input_manager.h>

#include <tiny-libc/tiny_libc.h>


static void update(void* _self, SenseInput* target)
{
    SenseXInputManager* self = (SenseXInputManager*) _self;
    senseXInputManagerUpdate(self, target);
}

void senseInputManagerCreatePlatformDriver(SenseInputManager* target, bl_size2i screen_size)
{
    SenseXInputManager* self = tc_malloc_type(SenseXInputManager);
    senseXInputManagerInit(self, screen_size);

    target->self = self;
    target->update_fn = update;
}
