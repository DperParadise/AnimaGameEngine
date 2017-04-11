#ifndef __APPLICATION_CPP__
#define __APPLICATION_CPP__

#include<list>
#include "Globals.h"
#include "Module.h"
#include "TimerMillis.h"
#include "TimerMicros.h"
#include <string>
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

	bool ReadConfigFile(const std::string &file);

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
	unsigned time_accumulated = 0;
	unsigned average_fps = 0;
	float ms_last_update = 0.0f;
	unsigned fps = 0;
	unsigned fps_cap = 0;
	Uint32 wait_time = 0;

	//dt in seconds
	float dt = 0.0f;

private:

	std::list<Module*> modules;
};

extern Application* App;

#endif // __APPLICATION_CPP__