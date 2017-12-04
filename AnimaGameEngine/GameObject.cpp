#include "GameObject.h"
#include "Globals.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentLight.h"
#include "ComponentAmbientLight.h"
#include "ComponentMeshRenderer.h"
#include "ComponentBehaviour.h"
#include "libraries/MathGeoLib/MathGeoLib.h"

//test
#include "ComponentTorsoBehaviour.h"

GameObject::GameObject(const std::string &name, aiNode *node) : name(name) , transform(this)
{
	if (node)
		LoadTransform(node);
	else
	{
		transform.relative_position.Set(0.0f, 0.0f, 0.0f);	
		
		transform.relative_scale.Set(1.0f, 1.0f, 1.0f);	
		
		transform.relative_rotation.x = 0.0f;
		transform.relative_rotation.y = 0.0f;
		transform.relative_rotation.z = 0.0f;
		transform.relative_rotation.w = 1.0f;

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
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		(*it)->Update(dt);
	}

	for (std::vector<GameObject*>::iterator it = children_go.begin(); it != children_go.end(); it++)
	{
		(*it)->Update(dt);
	}

	if (game_object_selected)
		gizmo.Draw(this);
}

void GameObject::UpdateWorldTransform()
{
	if (parent_go && parent_go->dirty)	
		dirty = true;

	if (dirty)
	{
		CombineTransform(parent_go);
		//UpdateBaseVectors(transform.world_rotation);
	}

	for (std::vector<GameObject*>::iterator it = children_go.begin(); it != children_go.end(); it++)
	{
		(*it)->UpdateWorldTransform();
	}

	dirty = false;
}

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

Component* GameObject::CreateMeshRenderer(ComponentMesh *mesh_comp)
{
	Component *comp = new ComponentMeshRenderer(ComponentType::MESH_RENDERER, mesh_comp, std::string("Mesh Renderer"), true, this);
	components.push_back(comp);
	return comp;
}

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

Component *GameObject::CreateDirectionalLight()
{
	Component *comp = new ComponentLight(ComponentType::DIRECTIONAL_LIGHT, std::string("Directional Light"), true, this);
	components.push_back(comp);
	return comp;
}

void GameObject::LoadTransform(aiNode *node)
{
	node->mTransformation.Decompose(transform.relative_scale, transform.relative_rotation, transform.relative_position);
}

void GameObject::CombineTransform(GameObject *parent_go)
{ 
	if (!parent_go)
	{
		transform.world_position = transform.relative_position;
		transform.world_scale = transform.relative_scale;
		transform.world_rotation = transform.relative_rotation;

		return;
	}

	transform.world_position = parent_go->transform.world_position + transform.relative_position;
	transform.world_scale = parent_go->transform.world_scale.SymMul(transform.relative_scale);
	transform.world_rotation = parent_go->transform.world_rotation * transform.relative_rotation;
}

void GameObject::UpdateBaseVectors(aiQuaternion rot)
{
	aiMatrix3x3 rot_matrix = rot.GetMatrix();
	transform.relative_forward.Set(rot_matrix.a3, rot_matrix.b3, rot_matrix.c3);
	transform.relative_left.Set(rot_matrix.a1, rot_matrix.b1, rot_matrix.c1);
	transform.relative_up.Set(rot_matrix.a2, rot_matrix.b2, rot_matrix.c2);

	//MYLOG("%s : relative_forward_z = (%f,%f,%f)  relative_left_x = (%f, %f, %f)  relative_up_y = (%f, %f, %f)", name.c_str(), transform.relative_forward.x, transform.relative_forward.y, transform.relative_forward.z,
	//	transform.relative_left.x, transform.relative_left.y, transform.relative_left.z, transform.relative_up.x, transform.relative_up.y, transform.relative_up.z);
}

//translation in relation to "this" base vectors
void GameObject::Transform::Translate(float local_x, float local_y, float local_z)
{
	aiVector3D translation = local_x * relative_left + local_y * relative_up + local_z * relative_forward;
	acum_rel_position += translation;
	relative_position.Set(relative_position.x + translation.x, relative_position.y + translation.y, relative_position.z + translation.z);
	owner_go->dirty = true;
}

void GameObject::Transform::Rotate(float x, float y, float z)
{
	//angle rotation sequence is z -> y -> x
	float3 rot_rad = DegToRad(float3(x, y, z));
	Quat rot = Quat::FromEulerZYX(rot_rad.z, rot_rad.y, rot_rad.x);
	aiQuaternion result = relative_rotation * aiQuaternion(rot.w, rot.x, rot.y, rot.z);
	relative_rotation = result;
	owner_go->UpdateBaseVectors(relative_rotation);
	owner_go->dirty = true;
}

void GameObject::Transform::Scale(float x, float y, float z)
{
	relative_scale.Set(x, y, z);
	owner_go->dirty = true;
}

const aiVector3D GameObject::Transform::local_forward = aiVector3D(0.0f, 0.0f, 1.0f);
const aiVector3D GameObject::Transform::local_left = aiVector3D(1.0f, 0.0f, 0.0f);
const aiVector3D GameObject::Transform::local_up = aiVector3D(0.0f, 1.0f, 0.0f);

void GameObject::Transform::ResetPosition(float rel_x, float rel_y, float rel_z)
{
	relative_position -= acum_rel_position;
	acum_rel_position = aiVector3D(0.0f, 0.0f, 0.0f);
	relative_position = aiVector3D(rel_x, rel_y, rel_z);
	owner_go->dirty = true;
}