#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <string>

enum class ComponentType
{
	CAMERA,
	MESH,
	MESH_RENDERER,
	MATERIAL,
	BEHAVIOUR,
	AMBIENT_LIGHT,
	POINT_LIGHT,
	DIRECTIONAL_LIGHT
};

class GameObject;
class Component
{
public:
	Component(ComponentType type, const std::string &name, bool act, GameObject *go);
	virtual ~Component();
	
	virtual void Update(float dt);
	virtual void Enable();
	virtual void Disable();

	ComponentType comp_type;
	std::string comp_name;
	bool active = true;
	GameObject *owner_go = nullptr;

};


#endif

