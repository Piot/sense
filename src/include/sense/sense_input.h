/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef sense_input_h
#define sense_input_h

#include <sense/sense_buttons.h>
#include <sense/sense_touches.h>

typedef struct SenseInput {
	SenseButtons devices[8];
	sense_touches touches;
} SenseInput;

#endif
