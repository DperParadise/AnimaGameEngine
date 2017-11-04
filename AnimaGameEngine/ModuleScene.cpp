#include "ModuleScene.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentLight.h"

ModuleScene::ModuleScene() {}

ModuleScene::~ModuleScene() {}

 bool ModuleScene::Start()
{	 
	 //create gameobjects

	 /*GameObject *grid_GO = CreateGameObject("grid primitive");
	 grid_GO->CreateComponent(component_type::TRANSFORM);
	 grid_GO->CreateComponent(component_type::GRID_MESH);*/

	 GameObject *gizmo_GO = CreateGameObject("gizmo");
	 gizmo_GO->CreateComponent(component_type::TRANSFORM);
	 gizmo_GO->CreateComponent(component_type::GIZMO_MESH);

	 GameObject *cube_GO = CreateGameObject("cube primitive");
	 cube_GO->CreateComponent(component_type::TRANSFORM);
	 cube_GO->CreateComponent(component_type::CUBE_MESH);
	 cube_GO->CreateComponent(component_type::MATERIAL);


	 /*GameObject *loaded_mesh_GO = CreateGameObject("loaded model");
	 loaded_mesh_GO->CreateComponent(component_type::TRANSFORM);
	 loaded_mesh_GO->CreateComponent(component_type::LOADED_MESH, "models/IronManFBX/IronMan.FBX");
	 loaded_mesh_GO->CreateComponent(component_type::MATERIAL, "models/IronManFBX/IronMan.FBX");
	 loaded_mesh_GO->CreateComponent(component_type::TEXTURE, "models/IronManFBX/IronMan.FBX");*/

	 GameObject *loaded_mesh_GO = CreateGameObject("loaded model");
	 loaded_mesh_GO->CreateComponent(component_type::TRANSFORM);
	 loaded_mesh_GO->CreateComponent(component_type::LOADED_MESH, "models/Batman/Batman.obj");
	 loaded_mesh_GO->CreateComponent(component_type::MATERIAL, "models/Batman/Batman.obj");
	 loaded_mesh_GO->CreateComponent(component_type::TEXTURE, "models/Batman/Batman.obj");
	 //--------------- LIGHTS ----------------------

	 /*GameObject *ambient_light_GO = CreateGameObject("ambient light");
	 ambient_light_GO->CreateComponent(component_type::AMBIENT_LIGHT);*/

	 /*GameObject *point_light_GO = CreateGameObject("point light 1");
	 Component *point_light1 = point_light1_GO->CreateComponent(component_type::LIGHT);
	 ((ComponentLight*)point_light1)->SetAmbient(0.2f, 0.2f, 0.2f, 1.0f);
	 ((ComponentLight*)point_light1)->SetDiffuse(0.8f, 0.8f, 0.8f, 1.0f);
	 ((ComponentLight*)point_light1)->SetSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	 ((ComponentLight*)point_light1)->SetPosition(0.0f, 2.0f, 3.0f, 1.0f);*/
	 
	 GameObject *spot_light_GO = CreateGameObject("spot light ");
	 Component *spot_light = spot_light_GO->CreateComponent(component_type::LIGHT);
	 ((ComponentLight*)spot_light)->SetAmbient(0.5f, 0.5f, 0.5f, 0.5f);
	 ((ComponentLight*)spot_light)->SetDiffuse(0.5f, 0.5f, 0.5f, 1.0f);
	 ((ComponentLight*)spot_light)->SetSpecular(0.5f, 0.5f, 0.5f, 1.0f);
	 ((ComponentLight*)spot_light)->SetPosition(0.0f, 0.0f, 5.0f);
	 ((ComponentLight*)spot_light)->SetSpotDirection(0.0f, 0.0f, -1.0f);
	

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