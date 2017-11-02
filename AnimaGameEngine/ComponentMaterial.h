#ifndef __COMPONENT_MATERIAL_H__
#define __COMPONENT_MATERIAL_H__

#include "Component.h"
#include <vector>
#include "libraries/glew-2.0.0/include/GL/glew.h"
#include "libraries/assimp/include/assimp/scene.h"
#include <string>

class GameObject;
class Importer;
class ComponentMaterial : public Component
{
public:
	ComponentMaterial(component_type t, bool act, GameObject *go, const char *file = nullptr);
	~ComponentMaterial();

	void Update();
	void Enable();
	void Disable();

	struct Material
	{
		GLfloat ambient[4] = { 0.0f,0.0f,0.0f,1.0f };
		GLfloat diffuse[4] = { 0.0f,0.0f,0.0f,1.0f };
		GLfloat specular[4] = { 0.0f,0.0f,0.0f,1.0f };
		GLfloat shininess = 32.0f;
	};

	std::vector<Material> materials;

private:
	const aiScene *scene = nullptr;
	Importer *importer = nullptr;
	void LoadMaterials(const char *file);
	
};
#endif
