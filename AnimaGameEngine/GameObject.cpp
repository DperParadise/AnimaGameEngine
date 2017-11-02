#include "GameObject.h"
#include "Globals.h"
#include "ComponentTransform.h"
#include "ComponentCubeMesh.h"
#include "ComponentGridMesh.h"
#include "ComponentGizmoMesh.h"
#include "ComponentSphereMesh.h"
#include "ComponentLoadedMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTexture.h"
#include "ComponentLight.h"
#include "ComponentAmbientLight.h"

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

Component *GameObject::CreateComponent(component_type type, const char *model_file)
{
	Component *comp = nullptr;
	switch (type)
	{
	case component_type::TRANSFORM:
		comp = new ComponentTransform(type, true, this);
		components.push_back(comp);
		break;

	case component_type::GRID_MESH:
		comp = new ComponentGridMesh(type, true, this);
		components.push_back(comp);
		break;

	case component_type::GIZMO_MESH:		
		comp = new ComponentGizmoMesh(type, true, this);
		components.push_back(comp);
		break;

	case component_type::CUBE_MESH:
		comp = new ComponentCubeMesh(type, true, this);
		components.push_back(comp);
		break;

	case component_type::SPHERE_MESH:
		comp = new ComponentSphereMesh(type, true, this);
		components.push_back(comp);
		break;

	case component_type::LOADED_MESH:
		comp = new ComponentLoadedMesh(model_file, type, true, this);
		components.push_back(comp);
		break;

	case component_type::MATERIAL:
		comp = new ComponentMaterial(type, true, this, model_file);
		components.push_back(comp);
		break;

	case component_type::TEXTURE:
		comp = new ComponentTexture(model_file, type, true, this);
		components.push_back(comp);
		break;

	case component_type::LIGHT:
		comp = new ComponentLight(ComponentLight::light_type::POINT, type, true, this);
		components.push_back(comp);
		break;

	case component_type::AMBIENT_LIGHT:
		comp = new ComponentAmbientLight(type, true, this);
		components.push_back(comp);
		break;
	}

	return comp;
}
