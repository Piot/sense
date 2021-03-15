/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef sense_webassembly_touches_h
#define sense_webassembly_touches_h

#include <basal/basal_size2i.h>
#include <sense/sense_touches.h>

typedef struct sense_webassembly_touches {
	sense_touches touches;
	bl_size2i screen_size;
} sense_webassembly_touches;

void sense_webassembly_touches_init(sense_webassembly_touches* self, const char* node, bl_size2i screen_size);
void sense_webassembly_touches_clear(sense_webassembly_touches* self);

#endif
