#include "ModuleScene.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentLight.h"
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

	 GameObject *world_origin = new GameObject("world origin");
	 world_origin->dirty = false;
	 AddGameObject(world_origin);

	 //CubeGO *cube_go = new CubeGO("Cube");	
	 //AddGameObject(cube_go);
	 Model street = Model("models/street/Street.obj", Model::load_flags::TRIANGULATE);
	 Model model_batman = Model("models/Batman/Batman.obj", Model::load_flags::FLIP_UVs | Model::load_flags::TRIANGULATE);
	  
	 //Model iron_man = Model("models/IronManFBX/IronMan.FBX", Model::load_flags::TRIANGULATE);
	 //Model magneto = Model("models/Magneto_obj_casco_solo/magneto_casco_solo.obj", Model::load_flags::TRIANGULATE);
	 

	 //test rotation of node g Line002
	 /*GameObject *crossroad = FindGameObject("g Line002");
	 int id = 0;
	 if (crossroad)
	 {
		 while (game_objects[0]->children_go.size() > 1)
		 {
			 if (game_objects[0]->children_go[id] != crossroad)
			 {
				 LinkGameObject(game_objects[0]->children_go[id], crossroad);
				 id = 0;
			 }
			 else
				 id++;
		 }	 
	 }*/

	return true;
}

update_status ModuleScene::Update(float dt) 
{
	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
	{
		(*it)->Update();

		(*it)->UpdateWorldTransform();


	}

	//if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT)
	//{
	//	//test rotation of node g Line002
	//	GameObject *crossroad = FindGameObject("g Line002");
	//	aiQuaternion rot;
	//	rot.x = 0.0f;
	//	rot.y = sin(0.5f * 0.01f);
	//	rot.z = 0.0f;
	//	rot.w = cos(0.5f * 0.01f);
	//	if (crossroad)
	//	{
	//		crossroad->transform.local_rotation = crossroad->transform.local_rotation * rot;
	//		crossroad->dirty = true;
	//	}
	//}

	int speed = 10;
	int scale_speed = 1;
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		aiVector3D pos = game_objects[2]->transform.local_position;

		aiVector3D dir = aiVector3D(game_objects[2]->transform.forward.x * speed * dt, 
			game_objects[2]->transform.forward.y * speed * dt,	
			game_objects[2]->transform.forward.z * speed * dt);

		game_objects[2]->transform.Translate(pos.x + dir.x, pos.y + dir.y, pos.z + dir.z);
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		aiVector3D pos = game_objects[2]->transform.local_position;

		aiVector3D dir = aiVector3D(game_objects[2]->transform.forward.x * -speed * dt,
			game_objects[2]->transform.forward.y * -speed * dt,
			game_objects[2]->transform.forward.z * -speed * dt);

		game_objects[2]->transform.Translate(pos.x + dir.x, pos.y + dir.y, pos.z + dir.z);
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		aiVector3D pos = game_objects[2]->transform.local_position;

		aiVector3D dir = aiVector3D(game_objects[2]->transform.left.x * speed * dt,
			game_objects[2]->transform.left.y * speed * dt,
			game_objects[2]->transform.left.z * speed * dt);

		game_objects[2]->transform.Translate(pos.x + dir.x, pos.y + dir.y, pos.z + dir.z);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		aiVector3D pos = game_objects[2]->transform.local_position;

		aiVector3D dir = aiVector3D(game_objects[2]->transform.left.x * -speed * dt,
			game_objects[2]->transform.left.y * -speed * dt,
			game_objects[2]->transform.left.z * -speed * dt);

		game_objects[2]->transform.Translate(pos.x + dir.x, pos.y + dir.y, pos.z + dir.z);
	}


	if (App->input->GetKey(SDL_SCANCODE_Y) == KEY_REPEAT)
	{
		game_objects[2]->transform.Rotate(0.0f, speed * dt, 0.0f);
	}
	if (App->input->GetKey(SDL_SCANCODE_U) == KEY_REPEAT)
	{
		game_objects[2]->transform.Rotate(0.0f, -speed * dt, 0.0f);
	}

	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_REPEAT)
	{
		aiVector3D scale = game_objects[2]->transform.local_scale;
		scale.x += scale_speed * dt;
		scale.y += scale_speed * dt;
		scale.z += scale_speed * dt;

		game_objects[2]->transform.Scale(scale.x, scale.y, scale.z);
	}
	if (App->input->GetKey(SDL_SCANCODE_N) == KEY_REPEAT)
	{
		aiVector3D scale = game_objects[2]->transform.local_scale;
		scale.x -= scale_speed * dt;
		scale.y -= scale_speed * dt;
		scale.z -= scale_speed * dt;

		game_objects[2]->transform.Scale(scale.x, scale.y, scale.z);
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
		return FindInHierarchy(name, *it);
	}
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
