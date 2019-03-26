#ifndef _COMP_MESH_RENDERER_H_
#define _COMP_MESH_RENDERER_H_

#include "Component.h"
#include <string>
#include <vector>
#include "Skeleton.h"

class Mesh;
class Shader;
class ComponentCamera;
class ModuleEditorCamera;

class ComponentMeshRenderer : public Component
{
public:
	ComponentMeshRenderer(	ComponentType type, 
							Mesh *mesh, 
							const Shader *shader,
							const ComponentCamera *compCamera,
							GameObject *ownerGO,
							bool act = true);
	~ComponentMeshRenderer();

	void Update(float dt);

	Mesh *GetMesh();

	void SetPoseMatrices(const std::vector<glm::mat4> &SkeletonPoses);
	
private:
	Mesh *mesh = nullptr;
	const Shader *shader = nullptr;
	const ComponentCamera *camera = nullptr;
	bool isPlaying = false;

	const glm::mat4 *skeletonPoses = nullptr;
	unsigned int poseVectorSize = 0;
};

#endif // !_COMP_MESH_RENDERER_H_
