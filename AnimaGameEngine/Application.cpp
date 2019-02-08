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
#include "ModuleEditorGUI.h"

#ifdef _MSC_VER
	#ifdef _DEBUG 
		#pragma comment(lib, "libraries/glew-2.0.0/libx86/deb/glew32d.lib")
		#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
		#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
		#pragma comment (lib, "libraries/DevIL_Windows_SDK/lib/x86/Release/DevIL.lib")
		#pragma comment (lib, "libraries/DevIL_Windows_SDK/lib/x86/Release/ILU.lib")
		#pragma comment (lib, "libraries/DevIL_Windows_SDK/lib/x86/Release/ILUT.lib")
		#pragma comment (lib, "3rdparty/lib/assimp-vc140-mt.lib")
		#pragma comment (lib, "3rdparty/lib/glfw3.lib")

	#else
		#pragma comment(lib, "libraries/glew-2.0.0/libx86/rel/glew32.lib")
		#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
		#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
		#pragma comment (lib, "libraries/DevIL_Windows_SDK/lib/x86/Release/DevIL.lib")
		#pragma comment (lib, "libraries/DevIL_Windows_SDK/lib/x86/Release/ILU.lib")
		#pragma comment (lib, "libraries/DevIL_Windows_SDK/lib/x86/Release/ILUT.lib")
		#pragma comment (lib, "libraries/assimp/lib/release/assimp-vc140-mt.lib")
		#pragma comment (lib, "3rdparty/lib/glfw3.lib")
	#endif
#endif

using namespace std;

Application::Application()
{
	// Order matters: they will init/start/pre/update/post in this order
	modules.push_back(input = new ModuleInput());
	modules.push_back(window = new ModuleWindow());
	modules.push_back(renderer = new ModuleRender()); 
	//TODO: Module textures not needed. It comes from legacy code in 2D engine
	//modules.push_back(textures = new ModuleTextures());  
	modules.push_back(audio = new ModuleAudio());
	//modules.push_back(module_editor_camera = new ModuleEditorCamera());
	modules.push_back(scene = new ModuleScene());
	//TODO: Under refactor to support opengl 3.3
	//modules.push_back(editor_gui = new ModuleEditorGUI()); 
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
	ms_cap = 1.0 / ((double)config->GetInt("Application", "fps_cap") / 1000.0);
	
	for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
	{
		ret = (*it)->Init(config); // we init everything, even if not anabled
	}

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
	{
		if((*it)->IsEnabled() == true)
			ret = (*it)->Start();
	}
	
	RELEASE(config);

	//------------------------------------- START TIMERS-----------------------------------
	timer_chrono.Start();

	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	//---------------------------------------   TIME CONTROL ---------------------------------------
	dt = timer_chrono.Read(); //milliseconds
	wait_time = 0.0;

	if (dt < ms_cap)
	{
		wait_time = ms_cap - dt;
		timer_chrono.Delay(wait_time);
	}

	dt /= 1000.0;

#ifdef _DEBUG
	//Comment out when debugging
	//ms_last_update = dt * 1000.0;
	//fps = 1.0 / dt;
	//MYLOG("MS LAST UPDATE = %f    WAIT TIME = %d    dt = %f    FPS = %d", ms_last_update, wait_time, dt, (int)fps);
#endif	
	
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



