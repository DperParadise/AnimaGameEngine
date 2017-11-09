#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <string>
#include <vector>
#include "Component.h"

struct aiNode;
struct aiMesh;
class GameObject
{
public:
	GameObject(const std::string &name);
	~GameObject();

	void Update();
	Component* CreateComponent(component_type type, const char *model_file = nullptr);

	Component* CreateTransformComp(aiNode *node);
	Component* CreateMeshComp(aiNode *node);
	Component* CreateMaterialComp(aiMesh *mesh);

	bool active = true;
	std::string name;
	std::vector<Component*> components;
	std::vector<GameObject*> children_go;
};



#endif
