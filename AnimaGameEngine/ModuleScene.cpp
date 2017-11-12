#include "ModuleScene.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentLight.h"
#include "Model.h"

ModuleScene::ModuleScene() {}

ModuleScene::~ModuleScene() {}

 bool ModuleScene::Start()
{	 
	 //create gameobjects

	 //Model model_batman = Model("models/Batman/Batman.obj");
	 //Model iron_man = Model("models/IronManFBX/IronMan.FBX");
	 //Model magneto = Model("models/Magneto_obj_casco_solo/magneto_casco_solo.obj");
	 Model street = Model("models/street/Street.obj");

	return true;
}

update_status ModuleScene::Update(float dt) 
{
	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
	{
		(*it)->Update();

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
