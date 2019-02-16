#ifndef MODULEWINDOW
#define MODULEWINDOW

#include "Module.h"

struct GLFWwindow;

class ModuleWindowGLFW : public Module
{
public:
	ModuleWindowGLFW();

	virtual bool Init(Config *config = nullptr);

	GLFWwindow *window = nullptr;
	int width = 0;
	int height = 0;

private:

};
#endif
