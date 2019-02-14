#include "GameObject.h"
#include "Globals.h"
#include "ComponentMeshRenderer.h"
#include "libraries/assimp/include/vector3.h"
#include "libraries/assimp/include/quaternion.h"
#include "libraries/assimp/include/scene.h"
#include "ComponentTransform.h"
#include "ComponentGizmo.h"
#include "ComponentCamera.h"
#include "ComponentEditorCamera.h"

GameObject::GameObject(const std::string &name, const aiNode *node) : name(name)
{
	transform = new ComponentTransform(ComponentType::TRANSFORM, this);
	AddGizmoComponent("shaders/gizmo.vert", "shaders/gizmo.frag"); //TODO: Implement a way to manage shaders

	if (node)
	{
		LoadTransform(node);
	}
}

void GameObject::LoadTransform(const aiNode *node)
{
	aiVector3D pos;
	aiVector3D scale;
	aiQuaternion rot;

	node->mTransformation.Decompose(scale, rot, pos);

	glm::vec3 posGLM(pos.x, pos.y, pos.z);
	glm::vec3 scaleGLM(scale.x, scale.y, scale.z);
	glm::quat rotGLM(rot.w, rot.x, rot.y, rot.z);

	transform->SetRelativePositionWorldAxis(posGLM);
	transform->SetRelativeScaleWorldAxis(scaleGLM);
	transform->SetRelativeRotationWorldAxis(rotGLM);
}

GameObject::~GameObject()
{
	Clear();
}

void GameObject::Update(float dt)
{
	if (!enabled)
		return;

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		(*it)->Update(dt);
	}

	for (std::vector<GameObject*>::iterator it = childrenGO.begin(); it != childrenGO.end(); it++)
	{
		(*it)->Update(dt);
	}
}

void GameObject::UpdateWorldTransform()
{
	if (!enabled)
		return;

	if (parentGO && parentGO->dirty)
		dirty = true;

	if (dirty)
	{
		CombineTransform(parentGO);

		for (std::vector<GameObject*>::iterator it = childrenGO.begin(); it != childrenGO.end(); it++)
		{
			(*it)->UpdateWorldTransform();
		}
	}

	dirty = false;
}

void GameObject::CombineTransform(GameObject *parentGO)
{

	//The world position will be the same as the relativePosition taken from the Asimp Decompose() method
	if (!parentGO)
	{
		transform->SetWorldPosition(transform->GetRelativePositionWorldAxis());
		transform->SetWorldScale(transform->GetRelativeScaleWorldAxis());
		transform->SetWorldRotation(transform->GetRelativeRotationWorldAxis());

		return;
	}

	transform->SetWorldPosition(parentGO->transform->GetWorldPosition() + transform->GetRelativePositionWorldAxis());
	transform->SetWorldScale(parentGO->transform->GetWorldScale() * transform->GetRelativeScaleWorldAxis());

	// TODO: Check quaternions mult order
	transform->SetWorldRotation(parentGO->transform->GetWorldRotation() * transform->GetRelativeRotationWorldAxis());
}

 ComponentTransform* GameObject::GetTransform()
{
	return transform;
}

 const ComponentTransform * GameObject::GetTransform() const
 {
	 return transform;
 }



const std::string & GameObject::GetName() const
{
	return name;
}

const std::vector<GameObject*>& GameObject::GetChildrenGO() const
{
	return childrenGO;
}

GameObject * GameObject::GetParentGO() const
{
	return parentGO;
}

void GameObject::SetParentGO(GameObject * parentGO)
{
	this->parentGO = parentGO;
}

void GameObject::AddChildGO(GameObject *childGO)
{
	if (childGO)
		childrenGO.push_back(childGO);
}


void GameObject::SetDirty()
{
	dirty = true;
}
bool GameObject::IsDirty() const
{
	return dirty;
}

const std::vector<Component*>& GameObject::GetComponents() const
{
	return components;
}

Component * GameObject::FindComponentByType(ComponentType type)
{
	Component *found = nullptr;
	for (Component *component : components)
	{
		if (component->GetComponentType() == type)
			found = component;
	}
	return found;
}
//Translation in world position
void GameObject::Translate(const glm::vec3 & pos)
{
	transform->Translate(pos);
	dirty = true;
}

void GameObject::Rotate(float angle, const glm::vec3 &axis)
{
	transform->Rotate(angle, axis);
	dirty = true;
}

void GameObject::Scale(const glm::vec3 & scale)
{
	transform->Scale(scale);
	dirty = true;
}


Component* GameObject::AddMeshRenderer(const Mesh *mesh, const Shader *shader, const ComponentCamera *camera)
{
	Component *comp = new ComponentMeshRenderer(ComponentType::MESH_RENDERER, mesh, shader, camera, this);
	components.push_back(comp);
	return comp;
}

Component * GameObject::AddGizmoComponent(const std::string & vertexPath, const std::string & fragmentPath)
{
	Component *comp = new ComponentGizmo(ComponentType::GIZMO, vertexPath, fragmentPath, this);
	components.push_back(comp);
	return comp;
}

Component * GameObject::AddCameraComponent()
{
	Component *comp = new ComponentCamera(ComponentType::CAMERA, this);
	components.push_back(comp);
	return comp;
}

Component * GameObject::AddEditorCameraComponent()
{
	Component *comp = new ComponentEditorCamera(ComponentType::CAMERA, this);
	components.push_back(comp);
	return comp;
}

void GameObject::Clear()
{
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		RELEASE(*it);
	}
	components.clear();

	for (std::vector<GameObject*>::iterator it = childrenGO.begin(); it != childrenGO.end(); it++)
	{
		RELEASE(*it);
	}
	childrenGO.clear();

	RELEASE(transform)
}
