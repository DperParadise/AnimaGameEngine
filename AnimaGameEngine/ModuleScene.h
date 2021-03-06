#ifndef __MODULESCENE_H__
#define __MODULESCENE_H__

#include "Module.h"
#include "Globals.h"
#include <vector>
#include <string>

class GameObject;
class EditorCameraGO;
class ComponentEditorCamera;
class ComponentCamera;
class Skeleton;
class Animation;

class ModuleScene : public Module
{
public:
	ModuleScene();
	virtual ~ModuleScene();
	virtual bool Start();
	virtual update_status Update(float dt);
	virtual bool CleanUp();

	void AddGameObject(GameObject *go);
	void AddSkeleton(Skeleton *skeleton);
	void SetActiveCamera(GameObject *camera);

	void OnResize(unsigned width, unsigned height);

	std::vector<GameObject*> gameObjects;
	std::vector<Skeleton*> skeletons;
	std::vector<Animation*> animations;
	GameObject *activeCameraGO = nullptr;
	ComponentCamera *activeCameraComponent = nullptr;
	EditorCameraGO *editorCameraGO = nullptr;

private:
	/*
	GameObject* CreateGameObject(const std::string &name);	
	GameObject* FindGameObject(const std::string &name);
	GameObject* FindInHierarchy(const std::string &name, GameObject *go);
	void LinkGameObject(GameObject *go, GameObject *dest);
	*/
};


#endif
