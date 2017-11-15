#include "GameObject.h"
#include "Globals.h"
#include "ComponentTransform.h"
#include "ComponentCubeMesh.h"
#include "ComponentGridMesh.h"
#include "ComponentGizmoMesh.h"
#include "ComponentSphereMesh.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTexture.h"
#include "ComponentLight.h"
#include "ComponentAmbientLight.h"
#include "ComponentMeshRenderer.h"

GameObject::GameObject(const std::string &name, aiNode *node) : name(name) 
{
	if (node)
		LoadTransform(node);
	else
	{
		transform.local_position.Set(0.0f, 0.0f, 0.0f);	
		
		transform.local_scale.Set(1.0f, 1.0f, 1.0f);	
		
		transform.local_rotation.x = 0.0f;
		transform.local_rotation.y = 0.0f;
		transform.local_rotation.z = 0.0f;
		transform.local_rotation.w = 1.0f;

		transform.world_position.Set(0.0f, 0.0f, 0.0f);
		
		transform.world_scale.Set(1.0f, 1.0f, 1.0f);

		transform.world_rotation.x = 0.0f;
		transform.world_rotation.y = 0.0f;
		transform.world_rotation.z = 0.0f;
		transform.world_rotation.w = 1.0f;
	}
}

GameObject::~GameObject()
{
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		RELEASE(*it);
	}
	components.clear();

	for (std::vector<GameObject*>::iterator it = children_go.begin(); it != children_go.end(); it++)
	{
		RELEASE(*it);
	}
	children_go.clear();
}

void GameObject::Update()
{
	if(game_object_selected)
		gizmo.Draw(this);

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		(*it)->Update();
	}

	for (std::vector<GameObject*>::iterator it = children_go.begin(); it != children_go.end(); it++)
	{
		(*it)->Update();
	}

}

void GameObject::UpdateWorldTransform()
{
	if (parent_go && parent_go->dirty)	
		dirty = true;

	if(dirty)
		CombineTransform(parent_go);

	for (std::vector<GameObject*>::iterator it = children_go.begin(); it != children_go.end(); it++)
	{
		(*it)->UpdateWorldTransform();
	}

	dirty = false;
}

//Component* GameObject::CreateComponent(component_type type, const char *model_file)
//{
//	Component *comp = nullptr;
//	switch (type)
//	{
//	case component_type::TRANSFORM:
//		comp = new ComponentTransform(type, true, this);
//		components.push_back(comp);
//		break;
//
//	case component_type::GRID_MESH:
//		comp = new ComponentGridMesh(type, true, this);
//		components.push_back(comp);
//		break;
//
//	case component_type::GIZMO_MESH:		
//		comp = new ComponentGizmoMesh(type, true, this);
//		components.push_back(comp);
//		break;
//
//	case component_type::CUBE_MESH:
//		comp = new ComponentCubeMesh(type, true, this);
//		components.push_back(comp);
//		break;
//
//	case component_type::SPHERE_MESH:
//		comp = new ComponentSphereMesh(type, true, this);
//		components.push_back(comp);
//		break;
//
//	case component_type::LOADED_MESH:
//		comp = new ComponentLoadedMesh(model_file, type, true, this);
//		components.push_back(comp);
//		break;
//
//	case component_type::MATERIAL:
//		comp = new ComponentMaterial(type, true, this, model_file);
//		components.push_back(comp);
//		break;
//
//	case component_type::TEXTURE:
//		comp = new ComponentTexture(model_file, type, true, this);
//		components.push_back(comp);
//		break;
//
//	case component_type::LIGHT:
//		comp = new ComponentLight(type, true, this);
//		components.push_back(comp);
//		break;
//
//	case component_type::AMBIENT_LIGHT:
//		comp = new ComponentAmbientLight(type, true, this);
//		components.push_back(comp);
//		break;
//	}
//
//	return comp;
//}

Component* GameObject::CreatePrimitiveMeshComp(ComponentMaterial *mat, float *vertices, float *normals, float *uv)
{
	Component *comp = new ComponentMesh(mat, component_type::MESH, true, this, vertices, normals, uv);
	components.push_back(comp);
	return comp;
}

Component* GameObject::CreatePrimitiveMatComp(float *ambient, float *diffuse, float *specular, float shininess)
{
	Component *comp = new ComponentMaterial(component_type::MATERIAL, true, this, ambient, diffuse, specular, shininess);
	components.push_back(comp);
	return comp;
}

Component* GameObject::CreateLoadedMeshComp(ComponentMaterial *mat, aiMesh *mesh)
{
	Component *comp = new ComponentMesh(mat, component_type::MESH, true, this, mesh);
	components.push_back(comp);
	return comp;
}

Component* GameObject::CreateLoadedMaterialComp(aiMesh *mesh, const aiScene *scene, const char *file_name)
{
	Component *comp = new ComponentMaterial(component_type::MATERIAL, true, this, mesh, scene, file_name);
	components.push_back(comp);
	return comp;
}

Component* GameObject::CreateMeshRenderer(ComponentMesh *mesh_comp)
{
	Component *comp = new ComponentMeshRenderer(mesh_comp, component_type::MESH_RENDERER, true, this);
	components.push_back(comp);
	return comp;
}

void GameObject::LoadTransform(aiNode *node)
{
	node->mTransformation.Decompose(transform.local_scale, transform.local_rotation, transform.local_position);
}

void GameObject::CombineTransform(GameObject *parent_go)
{ 
	if (!parent_go)
	{
		transform.world_position = transform.local_position;
		transform.world_scale = transform.local_scale;
		transform.world_rotation = transform.local_rotation;

		return;
	}

	transform.world_position = parent_go->transform.world_position + transform.local_position;
	transform.world_scale = parent_go->transform.world_scale.SymMul(transform.local_scale);
	transform.world_rotation = parent_go->transform.world_rotation * transform.local_rotation;
}
