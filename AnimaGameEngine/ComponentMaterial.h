#ifndef __COMPONENT_MATERIAL_H__
#define __COMPONENT_MATERIAL_H__

#include "Component.h"
#include "libraries/glew-2.0.0/include/GL/glew.h"
#include "libraries/assimp/include/assimp/types.h"

class GameObject;
struct aiMesh;
struct aiScene;
class ComponentMaterial : public Component
{
public:
	ComponentMaterial(component_type t, bool act, GameObject *go, aiMesh *mesh, aiScene *scene, const char *file_name);
	~ComponentMaterial();

	void Update();
	void Enable();
	void Disable();

	struct Material
	{
		GLfloat ambient[4] = { 0.5f,0.0f,0.0f,1.0f };
		GLfloat diffuse[4] = { 0.5f,0.0f,0.0f,1.0f };
		GLfloat specular[4] = { 0.5f,0.0f,0.0f,1.0f };
		GLfloat shininess = 32.0f;
	};

	Material material;
	aiString texture;

private:	
	void Load(aiMesh *mesh, aiScene *scene, const char *file_name);
	
};
#endif
