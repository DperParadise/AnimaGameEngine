#ifndef __APPLICATION_CPP__
#define __APPLICATION_CPP__

#include<list>
#include "Globals.h"
#include "TimerChrono.h"
#include <string>

class Module;
class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModuleEditorCamera;
class ModuleScene;
class ModuleEditorGUI;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleEditorCamera* module_editor_camera;
	ModuleScene* scene;
	ModuleEditorGUI* editor_gui;
	//------------------------------------------------- TIME CONTROL -----------------------------------------
	TimerChrono timer_chrono;
	double ms_last_update = 0.0f;
	double fps = 0.0f;
	double ms_cap = 0.0f;
	Uint32 wait_time = 0;

	//dt in seconds
	double dt = 0.0f;

private:

	std::list<Module*> modules;
};

extern Application* App;

#endif // __APPLICATION_CPP__