#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <string>

enum class ComponentType
{
	TRANSFORM,
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
	Component(ComponentType type, const std::string &name, GameObject *go, bool act = true);
	virtual ~Component();
	
	virtual void Update(float dt);
	virtual void Enable();
	virtual void Disable();

	ComponentType GetComponentType() const;
	const std::string &GetComponentName() const;
	bool IsActive() const;
	const GameObject* GetOwnerGO() const;

private:
	ComponentType type;
	std::string name;
	bool active = true;
	GameObject *ownerGO = nullptr;

};


#endif

