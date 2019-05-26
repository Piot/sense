
#include <sense/sense_input_manager.h>
#include <sense/sense_input.h>



int main(int argc, const char*argv[])
{
    bl_size2i screen;
    screen.height = 200;
    screen.width = 320;
    sdl2_init(screen);
    sense_input_driver driver;
    
    sense_input_manager_create_platform_driver(&driver, screen);
    while (1) {
        sense_input input;
        sense_input_manager_update(&driver, &input);
    }
}