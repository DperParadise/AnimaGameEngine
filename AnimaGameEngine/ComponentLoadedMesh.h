#ifndef __COMPONENT_LOADED_MESH_H__
#define __COMPONENT_LOADED_MESH_H__

#include "Component.h"

class GameObject;
struct aiMesh;
class ComponentLoadedMesh : public Component
{
public:
	ComponentLoadedMesh(component_type t, bool act, GameObject *go, aiMesh *mesh);
	~ComponentLoadedMesh();

	void Update();
	void Enable();
	void Disable();
	
private:
	
	unsigned int num_vertices = 0;

	float *vertex_array = nullptr;
	float *normal_array = nullptr;
	float *uv_array = nullptr;

	void Load(aiMesh *mesh);
};




#endif
