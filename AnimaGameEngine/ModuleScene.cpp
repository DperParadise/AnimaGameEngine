#include "ModuleScene.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentLight.h"

ModuleScene::ModuleScene() {}

ModuleScene::~ModuleScene() {}

 bool ModuleScene::Start()
{	 
	 //create gameobjects

	 GameObject *grid_GO = CreateGameObject("grid primitive");
	 grid_GO->CreateComponent(component_type::TRANSFORM);
	 grid_GO->CreateComponent(component_type::GRID_MESH);

	 GameObject *gizmo_GO = CreateGameObject("gizmo");
	 gizmo_GO->CreateComponent(component_type::TRANSFORM);
	 gizmo_GO->CreateComponent(component_type::GIZMO_MESH);

	 GameObject *cube_GO = CreateGameObject("cube primitive");
	 cube_GO->CreateComponent(component_type::TRANSFORM);
	 cube_GO->CreateComponent(component_type::CUBE_MESH);
	 cube_GO->CreateComponent(component_type::MATERIAL);

	 
	 /*GameObject *sphere_GO = CreateGameObject("sphere primitive");
	 sphere_GO->CreateComponent(component_type::TRANSFORM);
	 sphere_GO->CreateComponent(component_type::SPHERE_MESH);*/

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

	 /*GameObject *point_light1_GO = CreateGameObject("point light 1");
	 Component *point_light1 = point_light1_GO->CreateComponent(component_type::LIGHT);
	 ((ComponentLight*)point_light1)->light_t = ComponentLight::light_type::POINT;
	 ((ComponentLight*)point_light1)->SetAmbient(0.2f, 0.2f, 0.2f, 1.0f);
	 ((ComponentLight*)point_light1)->SetDiffuse(0.8f, 0.8f, 0.8f, 1.0f);
	 ((ComponentLight*)point_light1)->SetSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	 ((ComponentLight*)point_light1)->SetPosition(0.0f, 2.0f, 3.0f, 1.0f);*/
	 

	 GameObject *point_light2_GO = CreateGameObject("point light 2");
	 Component *point_light2 = point_light2_GO->CreateComponent(component_type::LIGHT);
	 ((ComponentLight*)point_light2)->light_t = ComponentLight::light_type::POINT;
	 ((ComponentLight*)point_light2)->SetAmbient(0.5f, 0.5f, 0.5f, 0.5f);
	 ((ComponentLight*)point_light2)->SetDiffuse(0.5f, 0.5f, 0.5f, 1.0f);
	 ((ComponentLight*)point_light2)->SetSpecular(0.5f, 0.5f, 0.5f, 1.0f);
	 ((ComponentLight*)point_light2)->SetPosition(0.0f, 0.0f, 1.0f, 1.0f);
	
/*
	 float ambient[] = { 0.0f, 0.0f, 0.0f, 0.1f };
	 float diffuse[] = { 1.0f, 1.0f, 1.0f, 0.1f };
	 float specular[] = { 1.0f, 1.0f, 1.0f, 0.1f };
	 float position[] = { 0.0f, 1.0f, 1.0f, 0.0f };

	 glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	 glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	 glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	 glLightfv(GL_LIGHT0, GL_POSITION, position);

	 GLfloat amb[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	 glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);

	 glEnable(GL_LIGHT0);
	 glEnable(GL_LIGHTING);
*/
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