#ifndef __COMPONENT_GIZMO_MESH_H__
#define __COMPONENT_GIZMO_MESH_H__

#include "Component.h"

class ComponentGizmoMesh : public Component
{
public:
	ComponentGizmoMesh(component_type t, bool act, GameObject *go);
	virtual ~ComponentGizmoMesh();

	virtual void Update(float dt);
	virtual void Enable();
	virtual void Disable();
};




#endif
