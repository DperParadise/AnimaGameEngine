#include "ModuleWindowGLFW.h"
#include <GLFW/glfw3.h>
#include "Config.h"

ModuleWindowGLFW::ModuleWindowGLFW(){}

bool ModuleWindowGLFW::Init(Config * config)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	MYLOG("Load ModuleWindow configuration");
	const char *windowTitle = config->GetString("ModuleWindow", "window_title");
	width = config->GetInt("ModuleWindow", "window_width");
	height = config->GetInt("ModuleWindow", "window_height");

	window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);
	if (window == NULL)
	{
		MYLOG("Failed to create GLFW window")
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	return true;
}
