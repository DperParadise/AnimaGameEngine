#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <string>

class GameObject;
class Component
{
public:
	Component(const std::string &name, bool act, GameObject *go);
	virtual ~Component();
	
	virtual void Update(float dt) = 0;
	virtual void Enable() = 0;
	virtual void Disable() = 0;

	std::string comp_name;
	bool active = true;
	GameObject *owner_go = nullptr;

};


#endif

