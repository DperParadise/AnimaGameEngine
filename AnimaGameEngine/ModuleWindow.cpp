#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "libraries/SDL/include/SDL.h"
#include "libraries/parson/parson.h"
#include "Config.h"

ModuleWindow::ModuleWindow()
{
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init(Config *config)
{
	MYLOG("Load ModuleWindow configuration");
	window_title = config->GetString("ModuleWindow", "window_title");
	window_width = config->GetInt("ModuleWindow", "window_width");
	window_height = config->GetInt("ModuleWindow", "window_height");
	full_screen = config->GetBool("ModuleWindow", "full_screen");
	resizable = config->GetBool("ModuleWindow", "resizable");

	MYLOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		MYLOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;

		if(full_screen == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}
		if (resizable == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		window = SDL_CreateWindow(window_title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, flags);

		if(window == nullptr)
		{
			MYLOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		

		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	MYLOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != nullptr)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::OnResize(int window_width, int window_height)
{
	this->window_width = window_width;
	this->window_height = window_height;
}


