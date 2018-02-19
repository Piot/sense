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
#import <GameController/GameController.h>
#include <sense/sense_input.h>
#include <tyran/tyran_log.h>

static GCController* findController()
{
	NSArray* controllers = [GCController controllers];

	GCController* foundController = 0;
	for (GCController* controller in controllers) {
		bool isValidGamepad = controller.gamepad || controller.extendedGamepad;
		if (isValidGamepad) {
			foundController = controller;
			break;
		}
	}
	return foundController;
}

static int value_to_int(float f)
{
	return (int)(f * 100.0f);
}

static int button_to_int(const GCControllerButtonInput* button)
{
	return value_to_int(button.value);
}

static void read_dpad(const GCControllerDirectionPad* dpad, sense_keyboard_keys* keys)
{
	const GCControllerButtonInput* left = dpad.left;
	const GCControllerButtonInput* right = dpad.right;
	const GCControllerButtonInput* up = dpad.up;
	const GCControllerButtonInput* down = dpad.down;
	keys->left = button_to_int(left);
	keys->right = button_to_int(right);
	keys->up = button_to_int(up);
	keys->down = button_to_int(down);
}

static void read_buttons( GCControllerButtonInput* a, const GCControllerButtonInput* b, sense_keyboard_keys* keys)
{
	keys->a = button_to_int(a);
	keys->b = button_to_int(b);
}

static void readController(const GCController* controller, sense_input* input)
{
	sense_keyboard_keys* keys = &input->keys;
	if (controller.extendedGamepad) {
		GCExtendedGamepad* egamepad = controller.extendedGamepad;
		const GCControllerDirectionPad* dpad = egamepad.dpad;
		read_dpad(dpad, keys);
		tyran_boolean any_digital_dpad = keys->left | keys->right | keys->up | keys->down;
		if (!any_digital_dpad) {
			read_dpad(egamepad.leftThumbstick, keys);
		}
		read_buttons(egamepad.buttonA, egamepad.buttonB, keys);
	} else if (controller.gamepad) {
		GCGamepad* gamepad = controller.gamepad;
		const GCControllerDirectionPad* dpad = gamepad.dpad;
		read_dpad(dpad, keys);
		read_buttons(gamepad.buttonA, gamepad.buttonB, keys);
	}
}


void sense_ios_input_gamepad_manager_update(sense_input* input)
{
	GCController* foundController = findController();
	if (!foundController) {
		return;
	}

	readController(foundController, input);
}
