#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "libraries/SDL/include/SDL.h"
#include "libraries/parson/parson.h"
#include "libraries/glew-2.0.0/include/GL/glew.h"
#include "libraries/SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Config.h"
#include "ModuleEditorCamera.h"


ModuleRender::ModuleRender()
{
	
}

// Destructor
ModuleRender::~ModuleRender()
{}

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
	glEnable(GL_CULL_FACE); //If disabled, back faces are visible but in grey colour
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);

	glViewport(0, 0, (GLint)App->window->window_width, (GLint)App->window->window_height);

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
	//---------------------------------------------- GIZMO (0,0,0) -----------------
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(3.0f, 0.0f, 0.0f);
	glVertex3i(0, 0, 0);
	glVertex3i(2, 0, 0);

	glColor3f(0.0f, 3.0f, 0.0f);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 2, 0);

	glColor3f(0.0f, 0.0f, 3.0);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 0, 2);
	glEnd();

	//---------------------------------------------- GRID DIRECT MODE ---------------------------------------
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	for (int z = -100; z <= 100; z += 1)
	{
		glVertex3i(-100, 0, z);
		glVertex3i(100, 0, z);
	}
	for (int x = -100; x <= 100; x += 1)
	{
		glVertex3i(x, 0, -100);
		glVertex3i(x, 0, 100);
	}
	
	glEnd();


	//---------------------------------------------- CUBE DIRECT MODE -----------------------------------------
	//glBegin(GL_TRIANGLES);
	//		
	//	//Front face
	//	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	//	
	//	glVertex3f(1.0f, 0.0f, 0.0f); 
	//	glVertex3f(1.0f, 1.0f, 0.0f); 
	//	glVertex3f(0.0f, 1.0f, 0.0f);
	//	
	//	glVertex3f(1.0f, 0.0f, 0.0f);
	//	glVertex3f(0.0f, 1.0f, 0.0f);
	//	glVertex3f(0.0f, 0.0f, 0.0f);

	//	//Upper face
	//	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	//
	//	glVertex3f(1.0f, 1.0f, 0.0f);
	//	glVertex3f(1.0f, 1.0f, 1.0f);
	//	glVertex3f(0.0f, 1.0f, 1.0f);

	//	glVertex3f(0.0f, 1.0f, 1.0f);
	//	glVertex3f(0.0f, 1.0f, 0.0f);
	//	glVertex3f(1.0f, 1.0f, 0.0f);

	//	//Right face
	//	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

	//	glVertex3f(1.0f, 0.0f, 0.0f);
	//	glVertex3f(1.0f, 0.0f, 1.0f);
	//	glVertex3f(1.0f, 1.0f, 0.0f);

	//	glVertex3f(1.0f, 1.0f, 0.0f);
	//	glVertex3f(1.0f, 0.0f, 1.0f);
	//	glVertex3f(1.0f, 1.0f, 1.0f);

	//	//Left face
	//	glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
	//	
	//	glVertex3f(0.0f, 1.0f, 0.0f);
	//	glVertex3f(0.0f, 0.0f, 1.0f);		
	//	glVertex3f(0.0f, 0.0f, 0.0f);

	//	glVertex3f(0.0f, 1.0f, 1.0f);
	//	glVertex3f(0.0f, 0.0f, 1.0f);		
	//	glVertex3f(0.0f, 1.0f, 0.0f);

	//	//Bottom face
	//	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);

	//	glVertex3f(0.0f, 0.0f, 1.0f);
	//	glVertex3f(1.0f, 0.0f, 1.0f);
	//	glVertex3f(1.0f, 0.0f, 0.0f);

	//	glVertex3f(1.0f, 0.0f, 0.0f);
	//	glVertex3f(0.0f, 0.0f, 0.0f);
	//	glVertex3f(0.0f, 0.0f, 1.0f);
	//	

	//glEnd();
	
	//---------------------------------------------- CUBE VERTEX ARRAYS WITH BUFFERS-----------------------------------------
	//GLfloat vertices[] =
	//{
	//	
	//	1.0f, 0.0f, 0.0f,
	//	1.0f, 1.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f,
	//	1.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f,
	//	0.0f, 0.0f, 0.0f,

	//	1.0f, 1.0f, 0.0f,
	//	1.0f, 1.0f, 1.0f,
	//	0.0f, 1.0f, 1.0f,
	//	0.0f, 1.0f, 1.0f,
	//	0.0f, 1.0f, 0.0f,
	//	1.0f, 1.0f, 0.0f
	//	
	//};

	//GLfloat colour[] =
	//{
	//	0.0f, 1.0f, 1.0f, 1.0f,
	//	0.0f, 1.0f, 1.0f, 1.0f,
	//	0.0f, 1.0f, 1.0f, 1.0f,
	//	0.0f, 1.0f, 1.0f, 1.0f,
	//	0.0f, 1.0f, 1.0f, 1.0f,
	//	0.0f, 1.0f, 1.0f, 1.0f,

	//	0.0f, 1.0f, 1.0f, 1.0f,
	//	0.0f, 1.0f, 1.0f, 1.0f,
	//	0.0f, 1.0f, 1.0f, 1.0f,
	//	0.0f, 1.0f, 1.0f, 1.0f,
	//	0.0f, 1.0f, 1.0f, 1.0f,
	//	0.0f, 1.0f, 1.0f, 1.0f,
	//};

	//uint my_id = 0;
	//glGenBuffers(1, (GLuint *)&(my_id));
	//glBindBuffer(GL_ARRAY_BUFFER, my_id);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12 * 3, vertices, GL_STATIC_DRAW);

	///*glEnableClientState(GL_COLOR_ARRAY);
	//glColorPointer(4, GL_FLOAT, 0, colour);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	//glDisableClientState(GL_COLOR_ARRAY);*/

	//glEnableClientState(GL_VERTEX_ARRAY);
	//glBindBuffer(GL_ARRAY_BUFFER, my_id);
	//glVertexPointer(3, GL_FLOAT, 0, NULL); //if no buffer is used pass NULL else pass vertices pointer
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	//glDisableClientState(GL_VERTEX_ARRAY);

	
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) 
	{
		float3 pos = App->module_editor_camera->frustum.Pos();
		pos.x -= 1.0f;
		App->module_editor_camera->SetPosition(pos);
		App->module_editor_camera->LookAt(float3(0.0f, 0.0f, 0.0f));
	}
	
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		float3 pos = App->module_editor_camera->frustum.Pos();
		pos.x += 1.0f;
		App->module_editor_camera->SetPosition(pos);
		App->module_editor_camera->LookAt(float3(0.0f, 0.0f, 0.0f));
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->module_editor_camera->viewMatrix);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(App->module_editor_camera->projectionMatrix);	
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

