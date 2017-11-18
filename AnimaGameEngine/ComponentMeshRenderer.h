#ifndef _COMP_MESH_RENDERER_H_
#define _COMP_MESH_RENDERER_H_

#include "Component.h"
#include <string>

class ComponentMesh;
class ComponentMeshRenderer : public Component
{
public:
	ComponentMeshRenderer(ComponentType type, ComponentMesh *mesh_comp, const std::string &name, bool act, GameObject *go);
	~ComponentMeshRenderer();

	void Update(float dt);
	void Enable();
	void Disable();

private:
	ComponentMesh *owner_mesh = nullptr;
};

#endif // !_COMP_MESH_RENDERER_H_
