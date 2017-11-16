#ifndef __COMPONENT_H__
#define __COMPONENT_H__

enum class component_type
{
	TRANSFORM,
	GRID_MESH,
	GIZMO_MESH,
	CUBE_MESH,
	SPHERE_MESH,
	MESH,
	MATERIAL,
	TEXTURE,
	LIGHT,
	AMBIENT_LIGHT,
	MESH_RENDERER,
	BEHAVIOUR
};

class GameObject;
class Component
{
public:
	Component(component_type t, bool act, GameObject *go);
	virtual ~Component();
	
	virtual void Update(float dt) = 0;
	virtual void Enable() = 0;
	virtual void Disable() = 0;

	component_type type;
	bool active = true;
	GameObject *owner_go = nullptr;

};


#endif

