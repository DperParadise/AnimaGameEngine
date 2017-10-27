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

private:
	std::vector<GameObject*> game_objects;
	
	GameObject* CreateGameObject(const std::string &name);

};


#endif
