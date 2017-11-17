#ifndef __MODULESCENE_H__
#define __MODULESCENE_H__

#include "Module.h"
#include "Globals.h"
#include <vector>
#include <string>

class GameObject;
class ModuleScene : public Module
{
public:
	ModuleScene();
	virtual ~ModuleScene();
	virtual bool Start();
	virtual update_status Update(float dt);
	virtual bool CleanUp();

	void AddGameObject(GameObject *go);

	std::vector<GameObject*> game_objects;

private:
	
	GameObject* CreateGameObject(const std::string &name);
	GameObject* FindGameObject(const std::string &name);
	GameObject* FindInHierarchy(const std::string &name, GameObject *go);
	void LinkGameObject(GameObject *go, GameObject *dest);
	
};


#endif
