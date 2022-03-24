/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef sense_webassembly_gamepad_h
#define sense_webassembly_gamepad_h

#include <sense/sense_keyboard_keys.h>

typedef struct sense_webassembly_gamepad {
    sense_keyboard_keys keys;
    bool device_in_focus;
    uint8_t bound_gamepad_index;
} sense_webassembly_gamepad;

void sense_webassembly_gamepad_init(sense_webassembly_gamepad* self);
void sense_webassembly_gamepad_poll(sense_webassembly_gamepad* self);

#endif
