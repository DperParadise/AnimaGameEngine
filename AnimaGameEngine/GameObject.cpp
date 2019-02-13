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

GameObject::~GameObject()
{
	Clear();
}

void GameObject::Update(float dt)
{
	if (!active)
		return;

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		(*it)->Update(dt);
	}

	for (std::vector<GameObject*>::iterator it = childrenGO.begin(); it != childrenGO.end(); it++)
	{
		(*it)->Update(dt);
	}

	//if (selectedGO)
	//	gizmo.Draw(this);
}

void GameObject::UpdateWorldTransform()
{
	if (!active)
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

void GameObject::Translate(const glm::vec3 & pos)
{
	transform->Translate(pos);
	dirty = true;
}

void GameObject::Scale(const glm::vec3 & scale)
{
	transform->Scale(scale);
	dirty = true;
}

 ComponentTransform* GameObject::GetTransform()
{
	return transform;
}

 const ComponentTransform * GameObject::GetTransform() const
 {
	 return transform;
 }

void GameObject::Rotate(float angle, const glm::vec3 &axis)
{
	transform->Rotate(angle, axis);
	dirty = true;
}
/*
Component* GameObject::CreatePrimitiveMeshComp(ComponentMaterial *mat, float *vertices, float *normals, float *uv)
{
	Component *comp = new ComponentMesh(ComponentType::MESH, mat, std::string("Mesh"), true, this, vertices, normals, uv);
	components.push_back(comp);
	return comp;
}

Component* GameObject::CreatePrimitiveMatComp(float *ambient, float *diffuse, float *specular, float shininess)
{
	Component *comp = new ComponentMaterial(ComponentType::MATERIAL, std::string("Material"), true, this, ambient, diffuse, specular, shininess);
	components.push_back(comp);
	return comp;
}

Component* GameObject::CreateLoadedMeshComp(ComponentMaterial *mat, aiMesh *mesh)
{
	Component *comp = new ComponentMesh(ComponentType::MATERIAL, mat, std::string("Mesh"), true, this, mesh);
	components.push_back(comp);
	return comp;
}

Component* GameObject::CreateLoadedMaterialComp(aiMesh *mesh, const aiScene *scene, const char *file_name)
{
	Component *comp = new ComponentMaterial(ComponentType::MATERIAL, std::string("Material"), true, this, mesh, scene, file_name);
	components.push_back(comp);
	return comp;
}
*/
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
/*
Component* GameObject::CreateBehaviour(const std::string &behav_name)
{
	Component *comp = new ComponentBehaviour(ComponentType::BEHAVIOUR, behav_name, std::string("Behaviour"), true, this);
	components.push_back(comp);
	return comp;
}

Component* GameObject::CreateTorsoBehaviour(const std::string &behav_name)
{
	Component *comp = new ComponentTorsoBehaviour(ComponentType::BEHAVIOUR, behav_name, std::string("TorsoBehaviour"), true, this);
	components.push_back(comp);
	return comp;
}

Component *GameObject::CreateAmbientLight()
{
	Component *comp = new ComponentAmbientLight(ComponentType::AMBIENT_LIGHT, std::string("Ambient Light"), true, this);
	components.push_back(comp);
	return comp;
}

Component *GameObject::CreatePointLight()
{
	Component *comp = new ComponentLight(ComponentType::POINT_LIGHT, std::string("Point Light"), true, this);
	components.push_back(comp);
	return comp;
}
*/
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

/*
Component *GameObject::CreateDirectionalLight()
{
	Component *comp = new ComponentLight(ComponentType::DIRECTIONAL_LIGHT, std::string("Directional Light"), true, this);
	components.push_back(comp);
	return comp;
}
*/
void GameObject::LoadTransform(const aiNode *node)
{
	//Assimp types
	aiVector3D pos;
	aiVector3D scale;
	aiQuaternion rot;

	node->mTransformation.Decompose(scale, rot, pos);

	// TODO: override asignment operator to assign between glm and assimp types
	//glm types
	glm::vec3 posGLM(pos.x, pos.y, pos.z);
	glm::vec3 scaleGLM(scale.x, scale.y, scale.z);
	glm::quat rotGLM(rot.x, rot.y, rot.z, rot.w);

	transform->SetRelativePositionWorldAxis(posGLM);
	transform->SetRelativeScaleWorldAxis(scaleGLM);
	transform->SetRelativeRotationWorldAxis(rotGLM);
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
