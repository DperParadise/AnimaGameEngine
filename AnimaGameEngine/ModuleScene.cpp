#include "ModuleScene.h"
#include "GameObject.h"
#include "Component.h"
//#include "ComponentLight.h"
//#include "ComponentAmbientLight.h"
#include "ModelLoader.h"
#include "CubeGO.h"
#include "EditorCameraGO.h"
#include "ComponentEditorCamera.h"
#include "ComponentCamera.h"
#include "Skeleton.h"
#include "Animation.h"

ModuleScene::ModuleScene() {}

ModuleScene::~ModuleScene() 
{
	RELEASE(editorCameraGO)
}

 bool ModuleScene::Start()
{	 
	//create gameobjects
	editorCameraGO = new EditorCameraGO("editor camera");
	editorCameraGO->FindComponentByType(ComponentType::GIZMO)->Disable();

	activeCameraGO = editorCameraGO;
	activeCameraComponent = (ComponentEditorCamera*)editorCameraGO->FindComponentByType(ComponentType::CAMERA);

	/*
	GameObject *world_origin = new GameObject("world origin");
	world_origin->dirty = false;
	AddGameObject(world_origin);
	*/
	/*
	GameObject *ambient_light = new GameObject("ambient light");
	ComponentAmbientLight *comp_amb_light = (ComponentAmbientLight*)ambient_light->CreateAmbientLight();
	AddGameObject(ambient_light);
	 
	GameObject *directional_light = new GameObject("dir light");
	ComponentLight *comp_dir_light = (ComponentLight*)directional_light->CreateDirectionalLight();
	AddGameObject(directional_light);
	*/
	/*GameObject *point_light = new GameObject("dir light");
	ComponentLight *comp_point_light = (ComponentLight*)point_light->CreateDirectionalLight();
	comp_point_light->SetPosition(0.0f, 0.0f, 20.0f);
	comp_point_light->SetDiffuse(1.0f, 1.0f, 0.0f, 1.0f);
	AddGameObject(point_light);*/

	//CubeGO *cube_go = new CubeGO("Cube");	
	//AddGameObject(cube_go);

	//Model street = Model("models/street/Street.obj", Model::load_flags::TRIANGULATE);
	//GameObject *batman = ModelLoader::Load("models/Batman/Batman.obj", ModelLoader::load_flags::FLIP_UVs | ModelLoader::load_flags::TRIANGULATE);
	//gameObjects.push_back(batman);
	//GameObject *batmanGO = FindGameObject("Batman.obj");
	//batmanGO->CreateBehaviour("BatmanMovement");
	//GameObject *torsoGO = FindGameObject("BatmanTorso");
	//torsoGO->CreateTorsoBehaviour("TorsoBehaviour");
	//Model iron_man = Model("models/IronMan/IronMan.obj", Model::load_flags::TRIANGULATE);
	//Model magneto = Model("models/Magneto_obj_casco_solo/magneto_casco_solo.obj", Model::load_flags::TRIANGULATE);
	 
	///Load animated model
	//Model stickFigure = Model("models/StickFigurea/StickFigurea.FBX", Model::load_flags::FLIP_UVs | Model::load_flags::TRIANGULATE);
 
	Skeleton *guardLampSkeleton = nullptr;
	GameObject *guardLampGO = ModelLoader::LoadMD5("models/GuardLamp/boblampclean.md5mesh", ModelLoader::load_flags::TRIANGULATE, &guardLampSkeleton, activeCameraComponent);
	gameObjects.push_back(guardLampGO);
	skeletons.push_back(guardLampSkeleton);
	//ModelLoader::LoadAnimationsMD5("models/GuardLamp/boblampclean.md5anim", animations, guardLampSkeleton);

	GameObject *dummy = new GameObject("dummy", nullptr);
	gameObjects.push_back(dummy);

	GameObject *worldOrigin = new GameObject("world origin", nullptr);
	gameObjects.push_back(worldOrigin);

	return true;
}

update_status ModuleScene::Update(float dt) 
{
	for (Animation *anim : animations)
	{
		anim->Update(dt);
	}

	//TODO: Improve skeletons management
	for (Skeleton *skl : skeletons)
	{
		skl->Update();
	}

	for (std::vector<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); it++)
	{
		(*it)->Update(dt);

		(*it)->UpdateWorldTransform();
	}

	activeCameraGO->Update(dt);

	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	for (std::vector<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); it++)
	{
		RELEASE(*it);
	}
	gameObjects.clear();
	
	for (Skeleton *skeleton : skeletons)
	{
		RELEASE(skeleton)
	}
	skeletons.clear();

	for (Animation *animation : animations)
	{
		RELEASE(animation)
	}
	animations.clear();

	return true;
}
/*
GameObject* ModuleScene::CreateGameObject(const std::string &name) 
{
	GameObject *go = new GameObject(name);
	gameObjects.push_back(go);

	return go;
}
*/
void ModuleScene::AddGameObject(GameObject *go)
{
	gameObjects.push_back(go);
}

void ModuleScene::AddSkeleton(Skeleton * skeleton)
{
	skeletons.push_back(skeleton);
}

void ModuleScene::SetActiveCamera(GameObject * camera)
{
	activeCameraGO = camera;
}
void ModuleScene::OnResize(unsigned width, unsigned height)
{
	ComponentCamera  *camera = nullptr;
	camera = (ComponentCamera*)activeCameraGO->FindComponentByType(ComponentType::CAMERA);

	if (camera)
		camera->OnResize(width, height);
}
/*
GameObject* ModuleScene::FindGameObject(const std::string &name) 
{
	
	for (std::vector<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); it++)
	{
		GameObject *found = nullptr;
		if ((found = FindInHierarchy(name, *it)) != nullptr)
			return found;
	}
	return nullptr;
}

 GameObject* ModuleScene::FindInHierarchy(const std::string &name,  GameObject *go) 
{
	 if (go->GetName() == name)
		 return go;

	 for each(GameObject *child in go->GetChildrenGO())
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
	
	for (auto it = go->GetParentGO()->GetChildrenGO().begin();  it != go->GetParentGO()->GetChildrenGO().end(); it++)
	{
		if ((*it) == go)
		{
			GameObject *found = *it;
			dest->AddChildGO(found);
			go->GetParentGO()->GetChildrenGO().erase(it); //TODO: create a GetChildrenGO() not const.
			found->SetParentGO(dest);
		
			break;
		}	
	}
}
*/