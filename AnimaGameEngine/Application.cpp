#include "Application.h"
#include "Module.h"
#include "ModuleWindowGLFW.h"
#include "ModuleRenderGLFW.h"
#include "ModuleTextures.h"
#include "ModuleInputGLFW.h"
#include "ModuleAudio.h"
#include "Config.h"
#include "ModuleEditorCamera.h"
#include "ModuleScene.h"
#include "ModuleEditorGUI.h"

#ifdef _MSC_VER
	#ifdef _DEBUG 
		#pragma comment(lib, "3rdparty/lib_x86/deb/glew32d.lib")
	#else
		#pragma comment(lib, "3rdparty/lib_x86/rel/glew32.lib")		
	#endif
		#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
		#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
		#pragma comment (lib, "3rdparty/lib_x86/DevIL.lib")
		#pragma comment (lib, "3rdparty/lib_x86/ILU.lib")
		#pragma comment (lib, "3rdparty/lib_x86/ILUT.lib")
		#pragma comment (lib, "3rdparty/lib_x86/assimp-vc140-mt.lib")
		#pragma comment (lib, "3rdparty/lib_x86/glfw3.lib")
#endif

using namespace std;

Application::Application()
{
	// Order matters: they will init/start/pre/update/post in this order
	modules.push_back(window = new ModuleWindowGLFW());
	modules.push_back(input = new ModuleInputGLFW());
	modules.push_back(scene = new ModuleScene());
	modules.push_back(renderer = new ModuleRenderGLFW());
	
	//TODO: Module textures not needed. It comes from legacy code in 2D engine
	//modules.push_back(textures = new ModuleTextures());  
	//modules.push_back(audio = new ModuleAudio());
	//modules.push_back(module_editor_camera = new ModuleEditorCamera());

	modules.push_back(editor_gui = new ModuleEditorGUI()); 
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

	//Initialize the time origin to use it in the next time readings inside Update()
	timer_chrono.SetTimeOrigin();

	return ret;
}

update_status Application::Update()
{
	int startUpdate = timer_chrono.ReadTime();

	update_status ret = UPDATE_CONTINUE;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PreUpdate(dt);

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->Update(dt);

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PostUpdate(dt);

	int endUpdate = timer_chrono.ReadTime();

	dt = endUpdate - startUpdate;
	wait_time = 0.0;

	if (dt < ms_cap)
	{
		wait_time = ms_cap - dt;
		timer_chrono.Delay(wait_time);
	}
	//In case we have waited time, we have to add this time to the dt
	dt += wait_time;

	//Convert dt to seconds
	dt /= 1000.0;

#ifdef _DEBUG
	//Comment out when debugging
	//ms_last_update = dt * 1000.0;
	//fps = 1.0 / dt;
	//MYLOG("MS LAST UPDATE = %f    WAIT TIME = %d    dt = %f    FPS = %f", ms_last_update, wait_time, dt, fps);
#endif	

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



