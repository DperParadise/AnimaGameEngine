#ifndef __COMPONENT_BEHAVIOUR_H__
#define __COMPONENT_BEHAVIOUR_H__

#include "Component.h"
#include <string>

//TODO : In run time: create a file, compile it as a dll and call its behaviour function inside the ComponentBehaviour::Update() 

class GameObject;
class ComponentBehaviour : public Component
{
public:
	ComponentBehaviour(ComponentType type, const std::string &behav_name, const std::string &name, bool act, GameObject *go);
	~ComponentBehaviour();

	 void Update(float dt);
	 void Enable();
	 void Disable();

	 int speed = 10;
	 int scale_speed = 1;

	 std::string behav_name;

};
#endif // !__COMPONENT_BEHAVIOUR_H__

