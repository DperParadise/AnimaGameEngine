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
	unsigned width = 0;
	unsigned height = 0;

private:

};
#endif
