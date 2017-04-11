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



ModuleRender::ModuleRender()
{
	camera.x = camera.y = 0;
	camera.w = SCREEN_WIDTH * SCREEN_SIZE;
	camera.h = SCREEN_HEIGHT* SCREEN_SIZE;
}

// Destructor
ModuleRender::~ModuleRender()
{}

// Called before render is available
bool ModuleRender::Init()
{
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
	
	glViewport(0, 0, (GLint)App->window->window_width, (GLint)App->window->window_height);

	//test glew Initialization
	MYLOG("Using Glew %s", glewGetString(GLEW_VERSION));
	MYLOG("Vendor: %s", glGetString(GL_VENDOR));
	MYLOG("Renderer: %s", glGetString(GL_RENDERER));
	MYLOG("OpenGL version supported %s", glGetString(GL_VERSION));
	MYLOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	//Init matrices
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
		
	//Check for error
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		MYLOG("Error initializing OpenGL! %s\n", gluErrorString(error));
		ret = false;
	}

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
	glClearColor(0.6f, 0.6f, 0.6f, 1.f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	//glEnable(GL_TEXTURE_2D);

	return ret;
}

update_status ModuleRender::PreUpdate()
{
	glViewport(0, 0, (GLint)App->window->window_width, (GLint)App->window->window_height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0.6f, 0.6f, 0.6f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}


update_status ModuleRender::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
		glColor3f(1, 0, 0); glVertex3f(-1.0f, -0.5f, -4.0f); // lower left vertex
		glColor3f(1, 0, 0); glVertex3f(1.0f, -0.5f, -4.0f); // lower right vertex
		glColor3f(1, 0, 0); glVertex3f(0.0f, 0.5f, -4.0f); // upper vertex
	glEnd();

	//SDL_RenderPresent(renderer);
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	MYLOG("Destroying renderer");

	//Destroy window
	if(renderer != nullptr)
	{
		SDL_DestroyRenderer(renderer);
	}

	return true;
}

// Blit to screen
bool ModuleRender::Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section, float speed)
{
	bool ret = true;
	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + x * SCREEN_SIZE;
	rect.y = (int)(camera.y * speed) + y * SCREEN_SIZE;

	if(section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= SCREEN_SIZE;
	rect.h *= SCREEN_SIZE;

	if(SDL_RenderCopy(renderer, texture, section, &rect) != 0)
	{
		MYLOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera)
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if (use_camera)
	{
		rec.x = (int)(camera.x + rect.x * SCREEN_SIZE);
		rec.y = (int)(camera.y + rect.y * SCREEN_SIZE);
		rec.w *= SCREEN_SIZE;
		rec.h *= SCREEN_SIZE;
	}

	if (SDL_RenderFillRect(renderer, &rec) != 0)
	{
		MYLOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::ReadConfigFile(const std::string &file)
{
	const char *c_file = file.c_str();
	bool ret = true;
	JSON_Value *root_value;
	JSON_Object *root_object;

	root_value = json_parse_file(c_file);
	if (root_value == NULL)
	{
		ret = false;
		return ret;
	}

	root_object = json_value_get_object(root_value);
	vsync = (bool)json_object_dotget_boolean(root_object, "ModuleRender.vsync");

	json_value_free(root_value);
	return ret;
}