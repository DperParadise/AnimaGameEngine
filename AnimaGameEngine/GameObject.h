#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <string>
#include <vector>
#include "Component.h"
#include "libraries/glm/glm.hpp"

struct aiNode;
class ComponentTransform;
class Mesh;
class Shader;
class ComponentCamera;

class GameObject
{
public:
	GameObject(const std::string &name, const aiNode *node = nullptr);
	virtual ~GameObject();

	virtual void Update(float dt);
	void UpdateWorldTransform();

	///Getters and setters
	ComponentTransform *GetTransform();
	const ComponentTransform *GetTransform() const;
	const std::string& GetName() const;
	const std::vector<GameObject*>& GetChildrenGO() const;
	GameObject *GetParentGO() const;
	void SetParentGO(GameObject *parentGO);
	void AddChildGO(GameObject *childGO);
	
	const std::vector<Component*>& GetComponents() const;
	Component *FindComponentByType(ComponentType type);
	
	void SetDirty();
	bool IsDirty() const;

	///GUI can modify this value
	bool enabled = true;


	///Methods to modify the transform
	void Translate(const glm::vec3 &translation);
	void Rotate(float angle, const glm::vec3 &axis);
	void Scale(const glm::vec3 &scale);

	///Methods to Add components
	Component* AddMeshRenderer(const Mesh *mesh, const Shader *shader, const ComponentCamera *camera);
	Component* AddCameraComponent();
	Component* AddEditorCameraComponent();
	Component* AddGizmoComponent(const std::string& vertexPath, const std::string &fragmentPath);

private:
	ComponentTransform *transform = nullptr;

	bool dirty = true;
	GameObject *parentGO = nullptr;
	std::string name;
	std::vector<Component*> components;
	std::vector<GameObject*> childrenGO;

	void LoadTransform(const aiNode *node);
	void CombineTransform(GameObject *parentGO);
	void Clear();
};



#endif
