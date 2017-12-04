#ifndef __TORSO_BEHAV_H__
#define __TORSO_BEHAV_H__

#include "Component.h"
#include <string>

//TODO : In run time: create a file, compile it as a dll and call its behaviour function inside the ComponentBehaviour::Update() 

class GameObject;
class ComponentTorsoBehaviour : public Component
{
public:
	ComponentTorsoBehaviour(ComponentType type, const std::string &behav_name, const std::string &name, bool act, GameObject *go);
	~ComponentTorsoBehaviour();

	void Update(float dt);
	void Enable();
	void Disable();

	int speed = 10;
	int scale_speed = 1;

	std::string behav_name;

};

#endif // !__TORSO_BEHAV_H__

