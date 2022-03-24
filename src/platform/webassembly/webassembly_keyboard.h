/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef sense_webassembly_keys_h
#define sense_webassembly_keys_h

#include <sense/sense_keyboard_keys.h>

typedef struct sense_webassembly_keys {
    sense_keyboard_keys keys[8];
    bool device_in_focus;
} sense_webassembly_keys;

void sense_webassembly_keys_init(sense_webassembly_keys* self, const char* node);

#endif
