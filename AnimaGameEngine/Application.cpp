#include "Application.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "Config.h"
#include "ModuleEditorCamera.h"
#include "ModuleScene.h"

#ifdef _MSC_VER
	#ifdef _DEBUG 
		#pragma comment(lib, "libraries/glew-2.0.0/libx86/deb/glew32d.lib")
		#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
		#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
		#pragma comment (lib, "libraries/DevIL_Windows_SDK/lib/x86/Release/DevIL.lib")
		#pragma comment (lib, "libraries/DevIL_Windows_SDK/lib/x86/Release/ILU.lib")
		#pragma comment (lib, "libraries/DevIL_Windows_SDK/lib/x86/Release/ILUT.lib")
		#pragma comment (lib, "libraries/assimp/lib/assimp-vc140-mt.lib")
	#else
		#pragma comment(lib, "libraries/glew-2.0.0/libx86/rel/glew32.lib")
		#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
		#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
		#pragma comment (lib, "libraries/DevIL_Windows_SDK/lib/x86/Release/DevIL.lib")
		#pragma comment (lib, "libraries/DevIL_Windows_SDK/lib/x86/Release/ILU.lib")
		#pragma comment (lib, "libraries/DevIL_Windows_SDK/lib/x86/Release/ILUT.lib")
		#pragma comment (lib, "libraries/assimp/lib/assimp-vc140-mt.lib")
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
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(audio = new ModuleAudio());

	modules.push_back(module_editor_camera = new ModuleEditorCamera());
	modules.push_back(scene = new ModuleScene());
	
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
	timerMillis.Start();
	timerMillis_accumulated.Start();

	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	//---------------------------------------   TIME CONTROL ---------------------------------------
	ms_last_update = timerMillis.Read();
	timerMillis.Start();
	dt = ms_last_update / 1000;
	fps = 1 / dt;

	if (fps_cap != 0)   
	{
		if (fps > fps_cap)
		{
			wait_time = 1000 / fps_cap - ms_last_update;
			SDL_Delay(wait_time);
		}
	}
	
	time_accumulated = timerMillis_accumulated.Read();
	frames_accumulated++;
	if(time_accumulated != 0)
		average_fps = 1000 * (float)frames_accumulated / time_accumulated;

	if (frames_accumulated == 1000)
	{
		frames_accumulated = 0;
		timerMillis_accumulated.Start();
	}

	//MYLOG("ACCUMULATED FRAMES = %d		TIME ACCUMULATED = %d		AVERAGE FPS = %f		MS LAST UPDATE = %f		  FPS = %d", frames_accumulated, time_accumulated, average_fps, ms_last_update, fps);
	
	
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



