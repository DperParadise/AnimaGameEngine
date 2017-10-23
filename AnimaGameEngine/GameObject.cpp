#include "GameObject.h"
#include "Globals.h"
#include "ComponentTransform.h"
#include "ComponentCubeMesh.h"
#include "ComponentGridMesh.h"
#include "ComponentGizmoMesh.h"
#include "ComponentSphereMesh.h"

GameObject::GameObject(const std::string &name) : name(name) {}

GameObject::~GameObject()
{
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		RELEASE(*it);
	}
	components.clear();
}

void GameObject::Update()
{
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		(*it)->Update();
	}
}

Component *GameObject::CreateComponent(component_type type)
{
	Component *comp = nullptr;
	switch (type)
	{
	case component_type::TRANSFORM:
		comp = new ComponentTransform(component_type::TRANSFORM, true, this);
		components.push_back(comp);
		break;

	case component_type::GRID_MESH:
		comp = new ComponentGridMesh(component_type::GRID_MESH, true, this);
		components.push_back(comp);
		break;

	case component_type::GIZMO_MESH:		
		comp = new ComponentGizmoMesh(component_type::GIZMO_MESH, true, this);
		components.push_back(comp);
		break;

	case component_type::CUBE_MESH:
		comp = new ComponentCubeMesh(component_type::CUBE_MESH, true, this);
		components.push_back(comp);
		break;

	case component_type::SPHERE_MESH:
		comp = new ComponentSphereMesh(component_type::SPHERE_MESH, true, this);
		components.push_back(comp);
		break;

	case component_type::LOADED_MESH:		
		break;

	case component_type::MATERIAL:
		break;
	}

	return comp;
}
