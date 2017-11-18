#include "GameObject.h"
#include "Globals.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentLight.h"
#include "ComponentAmbientLight.h"
#include "ComponentMeshRenderer.h"
#include "ComponentBehaviour.h"

GameObject::GameObject(const std::string &name, aiNode *node) : name(name) , transform(this)
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

void GameObject::Update(float dt)
{
	if(game_object_selected)
		gizmo.Draw(this);

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		(*it)->Update(dt);
	}

	for (std::vector<GameObject*>::iterator it = children_go.begin(); it != children_go.end(); it++)
	{
		(*it)->Update(dt);
	}

}

void GameObject::UpdateWorldTransform()
{
	if (parent_go && parent_go->dirty)	
		dirty = true;

	if (dirty)
	{
		CombineTransform(parent_go);
		UpdateBaseVectors(transform.world_rotation);
	}

	for (std::vector<GameObject*>::iterator it = children_go.begin(); it != children_go.end(); it++)
	{
		(*it)->UpdateWorldTransform();
	}

	dirty = false;
}

Component* GameObject::CreatePrimitiveMeshComp(ComponentMaterial *mat, float *vertices, float *normals, float *uv)
{
	Component *comp = new ComponentMesh(mat, std::string("Mesh"), true, this, vertices, normals, uv);
	components.push_back(comp);
	return comp;
}

Component* GameObject::CreatePrimitiveMatComp(float *ambient, float *diffuse, float *specular, float shininess)
{
	Component *comp = new ComponentMaterial(std::string("Material"), true, this, ambient, diffuse, specular, shininess);
	components.push_back(comp);
	return comp;
}

Component* GameObject::CreateLoadedMeshComp(ComponentMaterial *mat, aiMesh *mesh)
{
	Component *comp = new ComponentMesh(mat, std::string("Mesh"), true, this, mesh);
	components.push_back(comp);
	return comp;
}

Component* GameObject::CreateLoadedMaterialComp(aiMesh *mesh, const aiScene *scene, const char *file_name)
{
	Component *comp = new ComponentMaterial(std::string("Material"), true, this, mesh, scene, file_name);
	components.push_back(comp);
	return comp;
}

Component* GameObject::CreateMeshRenderer(ComponentMesh *mesh_comp)
{
	Component *comp = new ComponentMeshRenderer(mesh_comp, std::string("Mesh Renderer"), true, this);
	components.push_back(comp);
	return comp;
}

Component *GameObject::CreateBehaviour(const std::string &behav_name)
{
	Component *comp = new ComponentBehaviour(behav_name, std::string("Behaviour"), true, this);
	components.push_back(comp);
	return comp;
}

Component *GameObject::CreateAmbientLight()
{
	Component *comp = new ComponentAmbientLight(std::string("Ambient Light"), true, this);
	components.push_back(comp);
	return comp;
}

Component *GameObject::CreatePointLight()
{
	Component *comp = new ComponentLight(std::string("Point Light"), true, this);
	components.push_back(comp);
	return comp;
}

Component *GameObject::CreateDirectionalLight()
{
	Component *comp = new ComponentLight(std::string("Directional Light"), true, this);
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

void GameObject::UpdateBaseVectors(aiQuaternion world_rotation)
{
	aiMatrix3x3 rot_matrix = world_rotation.GetMatrix();
	transform.forward.Set(rot_matrix.a3, rot_matrix.b3, rot_matrix.c3);
	transform.left.Set(rot_matrix.a1, rot_matrix.b1, rot_matrix.c1);
	transform.up.Set(rot_matrix.a2, rot_matrix.b2, rot_matrix.c2);
}

void GameObject::Transform::Translate(float x, float y, float z)
{
	local_position.Set(x, y, z);
	owner_go->dirty = true;
}

void GameObject::Transform::Rotate(float x, float y, float z)
{
	aiQuaternion rot(y, z, x);
	aiQuaternion result = local_rotation*rot;
	local_rotation = result;
	owner_go->dirty = true;
}

void GameObject::Transform::Scale(float x, float y, float z)
{
	local_scale.Set(x, y, z);
	owner_go->dirty = true;
}


