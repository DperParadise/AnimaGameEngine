#ifndef __MODULEWINDOW_H__
#define __MODULEWINDOW_H__

#include "Module.h"
#include <string>

struct SDL_Window;
struct SDL_Surface;

class ModuleWindow : public Module
{

private:

	//---------------------- WINDOW CONFIGURATION-----------------------
	std::string window_title = "";
	bool full_screen = false;
	bool resizable = false;

public:

	unsigned window_width = 0;
	unsigned window_height = 0;

	ModuleWindow();

	virtual ~ModuleWindow();
	bool Init();	
	bool CleanUp();
	bool ReadConfigFile(const std::string &file);
	SDL_Window* window = nullptr;
	SDL_Surface* screen_surface = nullptr;

};

#endif // __MODULEWINDOW_H__