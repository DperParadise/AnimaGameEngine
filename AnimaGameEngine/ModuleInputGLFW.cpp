#include "ModuleInputGLFW.h"
#include "3rdparty/include/GLFW/glfw3.h"
#include "Application.h"
#include "ModuleWindowGLFW.h"
#include "ModuleScene.h"
#include "Globals.h"

//static variables
glm::vec2 ModuleInputGLFW::lastMousePos;
glm::vec2 ModuleInputGLFW::mouseOffset;
bool ModuleInputGLFW::firstMouseInteraction = true;
double ModuleInputGLFW::scrollOffsetY;

ModuleInputGLFW::ModuleInputGLFW()
{
}

bool ModuleInputGLFW::Init(Config * config)
{
	MYLOG("Use WASD + right click to travel inside the scene and rotate the camera with mouse movements")
	glfwSetFramebufferSizeCallback(App->window->window, FramebufferSizeCallback);
	glfwSetCursorPosCallback(App->window->window, MouseCallback);
	glfwSetScrollCallback(App->window->window, ScrollCallback);

	return true;
}

update_status ModuleInputGLFW::Update(float dt)
{
	if (glfwGetKey(App->window->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwTerminate();
		return update_status::UPDATE_STOP;
	}

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleInputGLFW::PostUpdate(float dt)
{
	mouseOffset = glm::vec2(0.0f);

	glfwPollEvents();

	return update_status::UPDATE_CONTINUE;
}

void ModuleInputGLFW::FramebufferSizeCallback(GLFWwindow * window, int width, int height)
{
	App->scene->OnResize(width, height);
}

void ModuleInputGLFW::MouseCallback(GLFWwindow * window, double xpos, double ypos)
{
	if (firstMouseInteraction)
	{
		lastMousePos.x = xpos;
		lastMousePos.y = ypos;
		firstMouseInteraction = false;
	}
	mouseOffset.x = xpos - lastMousePos.x;
	mouseOffset.y = ypos - lastMousePos.y;

	lastMousePos.x = xpos;
	lastMousePos.y = ypos;
}

void ModuleInputGLFW::ScrollCallback(GLFWwindow * window, double xoffset, double yoffset)
{
	scrollOffsetY = yoffset;
}

bool ModuleInputGLFW::KeyPressed(int key) const
{
	return glfwGetKey(App->window->window, key) == GLFW_PRESS;
}

bool ModuleInputGLFW::MouseButtonPressed(int button) const
{
	return glfwGetMouseButton(App->window->window, button) == GLFW_PRESS;
}

const glm::vec2 & ModuleInputGLFW::GetMouseOffset() const
{
	return mouseOffset;
}

double ModuleInputGLFW::GetScrollOffset() const
{
	return scrollOffsetY;
}
