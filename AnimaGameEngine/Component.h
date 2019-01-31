#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <string>

enum class ComponentType
{
	TRANSFORM,
	GIZMO,
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
	Component(ComponentType type, GameObject *owner, bool act = true);
	virtual ~Component();
	
	virtual void Update(float dt);
	virtual void Enable();
	virtual void Disable();

	ComponentType GetComponentType() const;
	bool IsActive() const;
	const GameObject* GetOwnerGO() const;

private:
	ComponentType type;
	bool active = true;
	GameObject *ownerGO = nullptr;

};


#endif

