#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "libraries/SDL/include/SDL.h"
#include "libraries/parson/parson.h"

ModuleWindow::ModuleWindow()
{
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	MYLOG("Init SDL window & surface");
	bool ret = true;

	if (!ReadConfigFile(CONFIG_FILE))
	{
		MYLOG("Error : cannot read config file");
		ret = false;
	}

	else if(SDL_Init(SDL_INIT_VIDEO) < 0)
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
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
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

bool ModuleWindow::ReadConfigFile(const std::string &file)
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
	window_title = json_object_dotget_string(root_object, "ModuleWindow.window_title");
	window_width = (unsigned)json_object_dotget_number(root_object, "ModuleWindow.window_width");
	window_height = (unsigned)json_object_dotget_number(root_object, "ModuleWindow.window_height");
	full_screen = (bool)json_object_dotget_boolean(root_object, "ModuleWindow.full_screen");
	resizable = (bool)json_object_dotget_boolean(root_object, "ModuleWindow.resizable");
	
	json_value_free(root_value);
	return ret;
}

