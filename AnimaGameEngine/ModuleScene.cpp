#include "ModuleScene.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentLight.h"
#include "ComponentAmbientLight.h"
#include "Model.h"
#include "CubeGO.h"

//test rotation
#include "Application.h"
#include "ModuleInput.h"
#include <cmath>

ModuleScene::ModuleScene() {}

ModuleScene::~ModuleScene() {}

 bool ModuleScene::Start()
{	 
	 //create gameobjects

	 /*GameObject *world_origin = new GameObject("world origin");
	 world_origin->dirty = false;
	 AddGameObject(world_origin);*/

	 /*GameObject *ambient_light = new GameObject("ambient light");
	 ComponentAmbientLight *comp_amb_light = (ComponentAmbientLight*)ambient_light->CreateAmbientLight();
	 AddGameObject(ambient_light);*/

	 GameObject *directional_light = new GameObject("dir light");
	 ComponentLight *comp_dir_light = (ComponentLight*)directional_light->CreateDirectionalLight();
	 AddGameObject(directional_light);

	 /*GameObject *point_light = new GameObject("dir light");
	 ComponentLight *comp_point_light = (ComponentLight*)point_light->CreateDirectionalLight();
	 comp_point_light->SetPosition(0.0f, 0.0f, 20.0f);
	 comp_point_light->SetDiffuse(1.0f, 1.0f, 0.0f, 1.0f);
	 AddGameObject(point_light);*/

	 //CubeGO *cube_go = new CubeGO("Cube");	
	 //AddGameObject(cube_go);

	 Model street = Model("models/street/Street.obj", Model::load_flags::TRIANGULATE);
	 /*Model model_batman = Model("models/Batman/Batman.obj", Model::load_flags::FLIP_UVs | Model::load_flags::TRIANGULATE);
	 GameObject *batmanGO = FindGameObject("Batman.obj");
	 batmanGO->CreateBehaviour("BatmanMovement");*/

	 //Model iron_man = Model("models/IronManFBX/IronMan.FBX", Model::load_flags::TRIANGULATE);
	 //Model magneto = Model("models/Magneto_obj_casco_solo/magneto_casco_solo.obj", Model::load_flags::TRIANGULATE);
	 

	return true;
}

update_status ModuleScene::Update(float dt) 
{
	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
	{
		(*it)->Update(dt);

		(*it)->UpdateWorldTransform();


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

void ModuleScene::AddGameObject(GameObject *go)
{
	game_objects.push_back(go);
}

GameObject* ModuleScene::FindGameObject(const std::string &name) 
{
	
	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
	{
		if (FindInHierarchy(name, *it) != nullptr)
			return *it;
	}
	return nullptr;
}

 GameObject* ModuleScene::FindInHierarchy(const std::string &name,  GameObject *go) 
{
	 if (go->name == name)
		 return go;

	 for each(GameObject *child in go->children_go)
	 {
		 if (FindInHierarchy(name, child) != nullptr)
			 return child;
	 }
	 return nullptr;
}

void ModuleScene::LinkGameObject(GameObject *go, GameObject *dest)
{
	if (go == nullptr || dest == nullptr)
		return;

	for (std::vector<GameObject*>::iterator it = go->parent_go->children_go.begin(); it != go->parent_go->children_go.end(); it++)
	{
		if ((*it) == go)
		{
			GameObject *found = *it;
			dest->children_go.push_back(found);
			go->parent_go->children_go.erase(it);
			found->parent_go = dest;
		
			break;
		}	
	}
}
