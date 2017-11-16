#ifndef __COMPONENT_BEHAVIOUR_H__
#define __COMPONENT_BEHAVIOUR_H__

#include "Component.h"
#include <string>

class ComponentBehaviour : public Component
{
public:
	ComponentBehaviour(std::string name, component_type t, bool act, GameObject *go);
	~ComponentBehaviour();

	 void Update(float dt);
	 void Enable();
	 void Disable();

	 int speed = 10;
	 int scale_speed = 1;

	 std::string name;

};
#endif // !__COMPONENT_BEHAVIOUR_H__

