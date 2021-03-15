/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <sense/sense_input.h>
#include <sense/sense_input_manager.h>

#include <breathe/breathe_app.h>
#include <clog/clog.h>
#include <stdio.h>

typedef struct example_app {
	SenseInputManager driver;
} example_app;

static example_app __app;

void* g_breathe_init(int argc, const char* argv[], int width, int height)
{
    printf("sense example init\n");
    bl_size2i screen_size;
    screen_size.width = width;
    screen_size.height = height;

	senseInputManagerCreatePlatformDriver(&__app.driver, screen_size);

    return &__app;
}

#define PRINT_BUTTON(name) printf("%d %s  ", input.keys[0].name, #name)

int g_breathe_draw(void* _app)
{
    example_app* app = _app;

	SenseInput input;
	senseInputManagerUpdate(&app->driver, &input);

	PRINT_BUTTON(a);
	PRINT_BUTTON(b);
	PRINT_BUTTON(x);
	PRINT_BUTTON(y);
	PRINT_BUTTON(menu);
	PRINT_BUTTON(up);
	PRINT_BUTTON(down);
	PRINT_BUTTON(left);
	PRINT_BUTTON(right);
	printf("\n");
	//CLOG_INFO("input a: %d", input.keys[0].a);

    int menu_pressed = input.keys[0].menu;
	int shouldQuit = menu_pressed;

    return shouldQuit;
}

void g_breathe_close(void* app)
{
    printf("close\n");
}

void g_breathe_touch(size_t id, sense_touch_phase phase, bl_vector2i position)
{
}

