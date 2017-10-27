#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <string>
#include <vector>
#include "Component.h"

class GameObject
{
public:
	GameObject(const std::string &name);
	~GameObject();

	void Update();
	Component* CreateComponent(component_type type, const char *model_file = nullptr);

	bool active = true;
	std::string name;
	std::vector<Component*> components;
};



#endif
