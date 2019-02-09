#ifndef _COMP_MESH_RENDERER_H_
#define _COMP_MESH_RENDERER_H_

#include "Component.h"
#include <string>


class Mesh;
class Shader;
class ComponentCamera;
class ModuleEditorCamera;

class ComponentMeshRenderer : public Component
{
public:
	ComponentMeshRenderer(	ComponentType type, 
							const Mesh *mesh, 
							const Shader *shader,
							const ComponentCamera *compCamera,
							GameObject *ownerGO,
							bool act = true);
	~ComponentMeshRenderer();

	void Update(float dt);

private:
	const Mesh *mesh = nullptr;
	const Shader *shader = nullptr;
	const ComponentCamera *camera = nullptr;
	bool isPlaying = false;



};

#endif // !_COMP_MESH_RENDERER_H_
