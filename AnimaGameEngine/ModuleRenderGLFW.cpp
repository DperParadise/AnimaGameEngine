#include "ModuleRenderGLFW.h"
#include "libraries/glew-2.0.0/include/GL/glew.h"
#include <GLFW/glfw3.h>
#include "Globals.h"
#include "Application.h"
#include "ModuleWindowGLFW.h"
#include "libraries/DevIL_Windows_SDK/include/IL/il.h"
#include "libraries/DevIL_Windows_SDK/include/IL/ilu.h"
#include "libraries/DevIL_Windows_SDK/include/IL/ilut.h"

bool ModuleRenderGLFW::Init(Config * config)
{
	//------------------------- INIT GLEW----------------------------------------------------
		
	GLenum initialization = glewInit();
	if (initialization != GLEW_OK)
	{
		MYLOG("Error on glewInit()");
		return false;
	}

	//------------- INIT DEVIL---------------------
	//Initialize IL
	ilInit();

	if (ilGetError() != IL_NO_ERROR)
		MYLOG(iluErrorString(ilGetError()));

	// Initialize ILU
	iluInit();

	if (ilGetError() != IL_NO_ERROR)
		MYLOG(iluErrorString(ilGetError()));

	// Initialize ILUT with OpenGL support.
	//ilutRenderer(ILUT_OPENGL);

	//if (ilGetError() != IL_NO_ERROR)
	//	MYLOG(iluErrorString(ilGetError()));

	return true;
}

update_status ModuleRenderGLFW::PreUpdate(float dt)
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleRenderGLFW::PostUpdate(float dt)
{
	glfwSwapBuffers(App->window->window);
	return update_status::UPDATE_CONTINUE;
}