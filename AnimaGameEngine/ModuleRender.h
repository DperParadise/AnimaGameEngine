//#ifndef __MODULERENDER_H__
//#define __MODULERENDER_H__
//
//#include "Module.h"
//#include "Globals.h"
//#include "libraries/SDL/include/SDL_video.h"
//
//struct SDL_Renderer;
//struct SDL_Rect;
//class Config;
//
//class ModuleRender : public Module
//{
//
//private:
//
//	//--------------------------- RENDER CONFIGURATION--------------------------------------
//	bool vsync = false;
//	SDL_GLContext gl_context = nullptr;
//
//public:
//
//	ModuleRender();
//	~ModuleRender();
//
//	bool Init(Config *config);
//	update_status PreUpdate(float dt);
//	update_status Update(float dt);
//	update_status PostUpdate(float dt);
//	bool CleanUp();
//
//	void OnResize();
//
//};
//
//#endif // __MODULERENDER_H__