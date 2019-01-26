#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <string>
#include <vector>
#include "Component.h"
#include "Gizmo.h"
#include "libraries/glm/glm.hpp"


struct aiNode;
struct aiMesh;
struct aiScene;
class ComponentMaterial;
class ComponentMesh;
class ComponentTransform;


class GameObject
{
public:

	GameObject(const std::string &name, const aiNode *node = nullptr);
	virtual ~GameObject();

	virtual void Update(float dt);
	void UpdateWorldTransform();

	void Translate(const glm::vec3 &translation);
	void Rotate(float angle, const glm::vec3 &axis);
	void Scale(const glm::vec3 &scale);

	const ComponentTransform *GetTransform() const;

	Component *CreatePrimitiveMeshComp(ComponentMaterial *mat, float *vertices, float *normals, float *uv);
	Component *CreatePrimitiveMatComp(float *ambient, float *diffuse, float *specular, float shininess);

	Component* CreateLoadedMeshComp(ComponentMaterial *mat, aiMesh *mesh);
	Component* CreateLoadedMaterialComp(aiMesh *mesh, const aiScene *scene, const char *file_name);
	Component* CreateMeshRenderer(ComponentMesh *mesh);
	Component* CreateBehaviour(const std::string &behav_name);
	Component* CreateTorsoBehaviour(const std::string &behav_name);
	Component* CreateAmbientLight();
	Component* CreateDirectionalLight();
	Component* CreatePointLight();

private:

	ComponentTransform *transform = nullptr;

	bool dirty = true;
	GameObject *parentGO = nullptr;
	bool active = true;
	std::string name;
	std::vector<Component*> components;
	std::vector<GameObject*> childrenGO;

	bool selectedGO = true;

	void LoadTransform(const aiNode *node);
	void CombineTransform(GameObject *parentGO);
	void Clear();
	Gizmo gizmo;
};



#endif
