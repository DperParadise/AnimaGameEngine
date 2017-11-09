#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <string>
#include <vector>
#include "Component.h"

struct aiNode;
struct aiMesh;
struct aiScene;
class GameObject
{
public:
	GameObject(const std::string &name);
	~GameObject();

	void Update();
	//Component* CreateComponent(component_type type, const char *model_file = nullptr);

	Component* CreateTransformComp(aiNode *node);
	Component* CreateMeshComp(aiMesh *mesh);
	Component* CreateMaterialComp(aiMesh *mesh, const aiScene *scene, const char *file_name);

	GameObject *parent_go = nullptr;
	bool active = true;
	std::string name;
	std::vector<Component*> components;
	std::vector<GameObject*> children_go;
};



#endif
