#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__

#include "Component.h"

class GameObject;
struct aiMesh;
class ComponentMaterial;
class ComponentMesh : public Component
{
public:
	ComponentMesh(ComponentMaterial *mat, component_type t, bool act, GameObject *go, aiMesh *mesh);
	ComponentMesh(ComponentMaterial *mat, component_type t, bool act, GameObject *go, float *vertices, float *normals, float *uv);
	~ComponentMesh();

	void Update();
	void Enable();
	void Disable();
	
	float *vertex_array = nullptr;
	float *normal_array = nullptr;
	float *uv_array = nullptr;
	ComponentMaterial *mesh_mat = nullptr;
	unsigned int num_vertices = 0;

private:	
	void Load(aiMesh *mesh);
};




#endif
