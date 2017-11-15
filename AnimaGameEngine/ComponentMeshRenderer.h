#ifndef _COMP_MESH_RENDERER_H_
#define _COMP_MESH_RENDERER_H_

#include "Component.h"

class ComponentMesh;
class ComponentMeshRenderer : public Component
{
public:
	ComponentMeshRenderer(ComponentMesh *mesh_comp, component_type t, bool act, GameObject *go);
	~ComponentMeshRenderer();

	void Update();
	void Enable();
	void Disable();

private:
	ComponentMesh *owner_mesh = nullptr;
};

#endif // !_COMP_MESH_RENDERER_H_
