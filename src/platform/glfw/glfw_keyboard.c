#include "glfw_keyboard.h"

#include <clog/clog.h>

static void onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}

void glfwKeyboardInit(GlfwKeyboard* self, GLFWwindow* window)
{
	//GLFWkeyfun previousCallback = glfwSetKeyCallback(window, onKey);
	//if (!previousCallback) {
	//	CLOG_ERROR("should always have previous callback");
	//}
	self->window = window;
}

#define CHECK_KEY(name) (glfwGetKey(win, name) == GLFW_PRESS) ? 1 : 0

size_t glfwKeyboardUpdate(GlfwKeyboard* self, SenseButtons* inputs, size_t maxInputs)
{
	struct GLFWwindow* win = self->window;
	if (maxInputs == 0) {
		return 0;
	}
	SenseButtons* first = &inputs[0];

	first->a = CHECK_KEY(GLFW_KEY_SPACE);
	first->b = CHECK_KEY(GLFW_KEY_E);
	first->x = CHECK_KEY(GLFW_KEY_Q);
	first->y = CHECK_KEY(GLFW_KEY_LEFT_SHIFT);
	first->menu = CHECK_KEY(GLFW_KEY_ESCAPE);
	first->up = CHECK_KEY(GLFW_KEY_W);
	first->down = CHECK_KEY(GLFW_KEY_S);
	first->left = CHECK_KEY(GLFW_KEY_A);
	first->right = CHECK_KEY(GLFW_KEY_D);

	return 1;
}