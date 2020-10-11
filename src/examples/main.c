
#include <sense/sense_input_manager.h>
#include <sense/sense_input.h>


#include <breathe/breathe_app.h>
#include <clog/clog.h>
#include <stdio.h>

typedef struct example_app {
    sense_input_driver driver;
} example_app;

static example_app __app;

void* g_breathe_init(int argc, const char* argv[], int width, int height)
{
    printf("sense example init\n");
    bl_size2i screen_size;
    screen_size.width = width;
    screen_size.height = height;
    sense_input_manager_create_platform_driver(&__app.driver, screen_size);
    return &__app;
}

int g_breathe_draw(void* _app)
{
    example_app* app = _app;
    sense_input input;
    sense_input_manager_update(&app->driver, &input);
	CLOG_INFO("input a: %d", input.keys[0].a);
    int menu_pressed = input.keys[0].menu;
    return menu_pressed;
}

void g_breathe_close(void* app)
{
    printf("close\n");
}

void g_breathe_touch(size_t id, sense_touch_phase phase, bl_vector2i position)
{
}

