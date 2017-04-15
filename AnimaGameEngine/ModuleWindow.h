#ifndef __MODULEWINDOW_H__
#define __MODULEWINDOW_H__

#include "Module.h"
#include <string>

struct SDL_Window;
struct SDL_Surface;
class Config;
class ModuleWindow : public Module
{

private:

	//---------------------- WINDOW CONFIGURATION-----------------------
	std::string window_title = "";
	bool full_screen = false;
	bool resizable = false;

public:

	int window_width = 0;
	int window_height = 0;

	ModuleWindow();

	virtual ~ModuleWindow();
	bool Init(Config *config);	
	bool CleanUp();
	SDL_Window* window = nullptr;
	SDL_Surface* screen_surface = nullptr;

};

#endif // __MODULEWINDOW_H__