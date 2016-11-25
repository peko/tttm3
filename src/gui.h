
#include <GLFW/glfw3.h>

#include "../dep/klib/kvec.h"
#include "types.h"

void gui_init(GLFWwindow* win, strings_v* n, void (*callback)(int));
void gui_logic(int width, int height);
void gui_draw();
void gui_cleanup();


