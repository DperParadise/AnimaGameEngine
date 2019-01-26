#ifndef _COMP_MESH_RENDERER_H_
#define _COMP_MESH_RENDERER_H_

#include "Component.h"
#include <string>


class Mesh;
class Shader;
class ComponentCamera;
class ModuleEditorCamera;

namespace glm
{
	class mat4;
}

class ComponentMeshRenderer : public Component
{
public:
	ComponentMeshRenderer(	ComponentType type, 
							const Mesh *mesh, 
							const Shader *shader,
							const ComponentCamera *compCamera,
							const ModuleEditorCamera *editorCam,
							const std::string &name, 
							bool act, 
							GameObject *ownerGO);
	~ComponentMeshRenderer();

	void Update(float dt);

	bool IsPlaying() const;
	void SetPlaying();

private:
	const Mesh *mesh = nullptr;
	const Shader *shader = nullptr;
	const ComponentCamera *sceneCamera = nullptr;
	const ModuleEditorCamera *editorCamera = nullptr;
	bool isPlaying = false;



};

#endif // !_COMP_MESH_RENDERER_H_
