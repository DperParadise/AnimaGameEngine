#include "ModuleRenderGLFW.h"
#include <glew_2.0.0/GL/glew.h>
#include <GLFW/glfw3.h>
#include "Globals.h"
#include "Application.h"
#include "ModuleWindowGLFW.h"
#include <devil/il.h>
#include <devil/ilu.h>
#include <devil/ilut.h>

bool ModuleRenderGLFW::Init(Config * config)
{
	//------------------------- INIT GLEW----------------------------------------------------
		
	GLenum initialization = glewInit();
	if (initialization != GLEW_OK)
	{
		MYLOG("Error on glewInit()");
		return false;
	}

	//test glew Initialization
	MYLOG("Using Glew %s", glewGetString(GLEW_VERSION));
	MYLOG("Vendor: %s", glGetString(GL_VENDOR));
	MYLOG("Renderer: %s", glGetString(GL_RENDERER));
	MYLOG("OpenGL version supported %s", glGetString(GL_VERSION));
	MYLOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

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

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	return true;
}

update_status ModuleRenderGLFW::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleRenderGLFW::PostUpdate(float dt)
{
	glfwSwapBuffers(App->window->window);
	return update_status::UPDATE_CONTINUE;
}
