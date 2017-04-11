#ifndef __APPLICATION_CPP__
#define __APPLICATION_CPP__

#include<list>
#include "Globals.h"
#include "Module.h"
#include "TimerMillis.h"
#include "TimerMicros.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;

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

	//------------------------------------------------- TIME CONTROL -----------------------------------------
	TimerMillis timerMillis;
	TimerMillis timerMillis_accumulated;
	TimerMicros timerMicros;

	unsigned frames_accumulated = 0;
	double time_accumulated = 0.0;
	unsigned average_fps = 0;
	double ms_last_update = 0;
	unsigned fps = 0;

private:

	std::list<Module*> modules;
};

extern Application* App;

#endif // __APPLICATION_CPP__