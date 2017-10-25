#ifndef __COMPONENT_LOADED_MESH_H__
#define __COMPONENT_LOADED_MESH_H__

#include "Component.h"
#include "libraries/assimp/include/assimp/scene.h"

class GameObject;
class Importer;
class ComponentLoadedMesh : public Component
{
public:
	ComponentLoadedMesh(const char *file, component_type t, bool act, GameObject *go);
	~ComponentLoadedMesh();

	void Update();
	void Enable();
	void Disable();
	
private:
	const aiScene *scene = nullptr;
	Importer *importer = nullptr;
	float **vertex_array = nullptr;
	float **normal_array = nullptr;

	void LoadMesh(const char *file);
};




#endif
