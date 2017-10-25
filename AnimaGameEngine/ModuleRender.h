#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"
#include "Globals.h"
#include "libraries/DevIL_Windows_SDK/include/IL/il.h"
#include <string>
#include "libraries/glew-2.0.0/include/GL/glew.h"
#include "Model.h"
#include "Cube.h"
#include "Grid.h"
#include "Gizmo.h"
#include "SpherePrimitive.h"

#include "GameObject.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;
class Config;
class ModuleRender : public Module
{

private:

	//--------------------------- RENDER CONFIGURATION--------------------------------------
	bool vsync = false;
	SDL_GLContext gl_context = nullptr;

	GLuint my_textures[2];
	ILuint image;
	ILubyte *image_data;
	GLuint my_buffers[3];
	Model imported_model;
	Cube cube_primitive;
	Grid grid_primitive;
	Gizmo gizmo_primitive;
	SpherePrimitive sphere_primitive;

	//test gameobjects
	GameObject *grid_GO = nullptr;
	GameObject *cube_GO = nullptr;
	GameObject *gizmo_GO = nullptr;
	GameObject *sphere_GO = nullptr;
	GameObject *loaded_mesh_GO = nullptr;

public:

	ModuleRender();
	~ModuleRender();

	bool Init(Config *config);
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int window_width, int window_height);

};

#endif // __MODULERENDER_H__