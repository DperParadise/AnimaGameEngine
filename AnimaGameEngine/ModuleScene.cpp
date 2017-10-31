#include "ModuleScene.h"
#include "GameObject.h"
#include "Component.h"

ModuleScene::ModuleScene() {}

ModuleScene::~ModuleScene() {}

 bool ModuleScene::Start()
{	 
	 //create gameobjects

	 GameObject *grid_GO = CreateGameObject("grid primitive");
	 grid_GO->CreateComponent(component_type::TRANSFORM);
	 grid_GO->CreateComponent(component_type::GRID_MESH);

	/* GameObject *cube_GO = CreateGameObject("cube primitive");
	 cube_GO->CreateComponent(component_type::TRANSFORM);
	 cube_GO->CreateComponent(component_type::CUBE_MESH);*/

	 GameObject *gizmo_GO = CreateGameObject("gizmo");
	 gizmo_GO->CreateComponent(component_type::TRANSFORM);
	 gizmo_GO->CreateComponent(component_type::GIZMO_MESH);

	 /*GameObject *sphere_GO = CreateGameObject("sphere primitive");
	 sphere_GO->CreateComponent(component_type::TRANSFORM);
	 sphere_GO->CreateComponent(component_type::SPHERE_MESH);*/

	 GameObject *loaded_mesh_GO = CreateGameObject("loaded model");
	 loaded_mesh_GO->CreateComponent(component_type::TRANSFORM);
	 loaded_mesh_GO->CreateComponent(component_type::LOADED_MESH, "models/Batman/Batman.obj");
	 loaded_mesh_GO->CreateComponent(component_type::MATERIAL, "models/Batman/Batman.obj");
	 loaded_mesh_GO->CreateComponent(component_type::TEXTURE, "models/Batman/Batman.obj");

	return true;
}

update_status ModuleScene::Update(float dt) 
{
	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
	{
		(*it)->Update();
	}
	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
	{
		RELEASE(*it);
	}
	game_objects.clear();
	
	return true;
}

GameObject* ModuleScene::CreateGameObject(const std::string &name) 
{
	GameObject *go = new GameObject(name);
	game_objects.push_back(go);

	return go;
}