#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "Config.h"
#include "ModuleEditorCamera.h"


#ifdef _MSC_VER
	#ifdef _DEBUG 
		#pragma comment(lib, "libraries/glew-2.0.0/libx86/deb/glew32d.lib")
		#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
		#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
	#else
		#pragma comment(lib, "libraries/glew-2.0.0/libx86/rel/glew32.lib")
		#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
		#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
	#endif
	
	#ifdef _WIN64
		#ifdef _DEBUG
			#pragma comment(lib, "libraries/MathGeoLib/libx64/deb/MathGeoLib.lib")
		#else
			#pragma comment(lib, "libraries/MathGeoLib/libx64/rel/MathGeoLib.lib")
		#endif
	#else
		#ifdef _DEBUG
			#pragma comment(lib, "libraries/MathGeoLib/libx86/deb/MathGeoLib.lib")
		#else
			#pragma comment(lib, "libraries/MathGeoLib/libx86/rel/MathGeoLib.lib")
		#endif
	#endif
#endif

using namespace std;

Application::Application()
{
	// Order matters: they will init/start/pre/update/post in this order
	modules.push_back(input = new ModuleInput());
	modules.push_back(window = new ModuleWindow());
	modules.push_back(module_editor_camera = new ModuleEditorCamera());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(audio = new ModuleAudio());
	
	
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		RELEASE(*it);
}

bool Application::Init()
{
	bool ret = true;

	Config *config = Config::LoadConfig(CONFIG_FILE);
	if (config == nullptr)
	{
		MYLOG("Error in configuration file.")
		return false;
	}

	MYLOG("Load Application configuration");
	fps_cap = config->GetInt("Application", "fps_cap");

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init(config); // we init everything, even if not anabled

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
	{
		if((*it)->IsEnabled() == true)
			ret = (*it)->Start();
	}

	RELEASE(config);

	//------------------------------------- START TIMERS-----------------------------------
	timerMicros.Start();
	timerMillis_accumulated.Start();

	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	//---------------------------------------   TIME CONTROL ---------------------------------------

	frames_accumulated++;
	time_accumulated = timerMillis_accumulated.Read();
	ms_last_update = timerMicros.Read();
	dt = ms_last_update / 1000.0f;
	if (ms_last_update < (1.0f / ((double)fps_cap / 1000.0f)))
	{
		double cap_time = (1.0f / ((double)fps_cap / 1000.0f));
		wait_time = (Uint32)(cap_time - ms_last_update);
		SDL_Delay(wait_time);
	}

	fps = (unsigned)(1.0f / ((ms_last_update + wait_time) / 1000.0f));
	average_fps = (unsigned)(frames_accumulated / ((time_accumulated + wait_time) / 1000.0f));
	wait_time = 0;
	timerMicros.Start();

	//MYLOG("ACCUMULATED FRAMES = %d		TIME ACCUMULATED = %d		AVERAGE FPS = %d		MS LAST UPDATE = %f		  FPS = %d", frames_accumulated, time_accumulated, average_fps, ms_last_update, fps);


	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PreUpdate(dt);

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->Update(dt);

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PostUpdate(dt);

	
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->CleanUp();

	return ret;
}



