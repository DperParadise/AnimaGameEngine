#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "libraries/parson/parson.h"


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

	
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		RELEASE(*it);
}

bool Application::Init()
{
	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init(); // we init everything, even if not anabled

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
	{
		if((*it)->IsEnabled() == true)
			ret = (*it)->Start();
	}
	//------------------------------------- START TIMERS-----------------------------------
	timerMicros.Start();
	timerMillis_accumulated.Start();

	//read fps_cap from config
	if (!ReadConfigFile(CONFIG_FILE))
	{
		MYLOG("Error : cannot read config file.");
	}

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
			ret = (*it)->PreUpdate();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->Update(dt);

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PostUpdate();

	
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

bool Application::ReadConfigFile(const std::string &file)
{
	const char *c_file = file.c_str();
	bool ret = true;
	JSON_Value *root_value;
	JSON_Object *root_object;

	root_value = json_parse_file(c_file);
	if (root_value == NULL)
	{
		ret = false;
		return ret;
	}

	root_object = json_value_get_object(root_value);
	fps_cap = (unsigned)json_object_dotget_number(root_object, "Application.fps_cap");
	json_value_free(root_value);
	return ret;
}

