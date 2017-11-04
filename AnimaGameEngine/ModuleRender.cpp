#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "libraries/SDL/include/SDL.h"
#include "libraries/parson/parson.h"
#include "libraries/glew-2.0.0/include/GL/glew.h"
#include "libraries/SDL/include/SDL_opengl.h"
#include "Config.h"
#include "ModuleEditorCamera.h"
#include "libraries/DevIL_Windows_SDK/include/IL/il.h"
#include "libraries/DevIL_Windows_SDK/include/IL/ilu.h"
#include "libraries/DevIL_Windows_SDK/include/IL/ilut.h"
#include "ModuleScene.h"
#include "ModuleEditorGUI.h"

ModuleRender::ModuleRender(){}

// Destructor
ModuleRender::~ModuleRender(){}

// Called before render is available
bool ModuleRender::Init(Config *config)
{
	MYLOG("Load ModuleRender configuration");
	vsync = config->GetBool("ModuleRender", "vsync");

	MYLOG("Creating Renderer context");
	bool ret = true;
	Uint32 flags = 0;

	if(vsync == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
	}

	//------------------------- CREATE OPENGL CONTEXT BEFORE INIT GLEW ------------------------

	gl_context = SDL_GL_CreateContext(App->window->window);

	if (gl_context == nullptr)
	{
		MYLOG("Error : cannot create openGL context");
		return false;
	}

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

	//Init projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Check for error
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		MYLOG("Error initializing OpenGL! %s\n", gluErrorString(error));
		ret = false;
	}

	//Init modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Check for error
	 error = glGetError();
	if (error != GL_NO_ERROR)
	{
		MYLOG("Error initializing OpenGL! %s\n", gluErrorString(error));
		ret = false;
	}

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearDepth(1.0f);
	glClearColor(0.0, 0.0f, 0.0f, 1.f);

	//Check for error
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		MYLOG("Error initializing OpenGL! %s\n", gluErrorString(error));
		ret = false;
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST); //If enabled, glClear(GL_DEPTH_BUFFER_BIT)  must be called in PreUpdate
	//glEnable(GL_CULL_FACE); //If disabled, back faces are visible
	glEnable(GL_TEXTURE_2D);

	glViewport(0, 0, (GLint)App->window->window_width, (GLint)App->window->window_height);


	//------------- INIT DEVIL---------------------
	//Initialize IL
	ilInit();
	if(ilGetError() != IL_NO_ERROR)
		MYLOG(iluErrorString(ilGetError()));

	// Initialize ILU
	iluInit();	if (ilGetError() != IL_NO_ERROR)
		MYLOG(iluErrorString(ilGetError()));	// Initialize ILUT with OpenGL support.
	ilutRenderer(ILUT_OPENGL);	if (ilGetError() != IL_NO_ERROR)
		MYLOG(iluErrorString(ilGetError()));

	
	return ret;
}

update_status ModuleRender::PreUpdate(float dt)
{	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}


update_status ModuleRender::Update(float dt)
{	
	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate(float dt)
{	
	//-------- DRAWING SCENE------------
	
	App->scene->Update(dt);
	

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->module_editor_camera->viewMatrix);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(App->module_editor_camera->projectionMatrix);	

	ImGui::Render();
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	MYLOG("Destroying renderer");

	//Destroy OpenGL context
	SDL_GL_DeleteContext(gl_context);

	return true;
}

void ModuleRender::OnResize(int window_width, int window_height)
{
	glViewport(0, 0, window_width, window_height);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(App->module_editor_camera->projectionMatrix);
}

