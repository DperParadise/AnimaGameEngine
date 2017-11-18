#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__

#include "Component.h"
#include <string>

class GameObject;
struct aiMesh;
class ComponentMaterial;
class ComponentMesh : public Component
{
public:
	ComponentMesh(ComponentMaterial *mat, const std::string &name, bool act, GameObject *go, aiMesh *mesh);
	ComponentMesh(ComponentMaterial *mat, const std::string &name, bool act, GameObject *go, float *vertices, float *normals, float *uv);
	~ComponentMesh();

	void Update(float dt);
	void Enable();
	void Disable();
	
	float *vertex_array = nullptr;
	float *normal_array = nullptr;
	float *uv_array = nullptr;
	ComponentMaterial *mesh_mat = nullptr;
	unsigned int num_vertices = 0;
	std::string mesh_name;

private:	
	void Load(aiMesh *mesh);
};




#endif
