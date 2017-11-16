#ifndef __COMPONENT_GRID_MESH_H__
#define __COMPONENT_GRID_MESH_H__

#include "Component.h"

class GameObject;
class ComponentGridMesh : public Component
{
public:
	ComponentGridMesh(component_type t, bool act, GameObject *go);
	virtual ~ComponentGridMesh();

	virtual void Update(float dt);
	virtual void Enable();
	virtual void Disable();

};




#endif
