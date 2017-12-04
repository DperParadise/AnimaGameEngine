#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <string>
#include <vector>
#include "Component.h"
#include "libraries/assimp/include/assimp/vector3.h"
#include "libraries/assimp/include/assimp/quaternion.h"
#include "libraries/assimp/include/assimp/scene.h"
#include "Gizmo.h"

struct aiNode;
struct aiMesh;
struct aiScene;
class ComponentMaterial;
class ComponentMesh;
class GameObject
{
public:

	struct Transform
	{
	public:
		Transform(GameObject *owner) : owner_go(owner) {}
		
		
		aiVector3D acum_rel_position = aiVector3D(0.0f, 0.0f, 0.0f);

		aiVector3D relative_position;
		aiVector3D relative_scale;
		aiQuaternion relative_rotation;

		aiVector3D world_position;
		aiVector3D world_scale;
		aiQuaternion world_rotation;
	
		GameObject *owner_go = nullptr;

		aiVector3D relative_forward = aiVector3D(0.0f, 0.0f, 1.0f);
		aiVector3D relative_left = aiVector3D(1.0f, 0.0f, 0.0f);
		aiVector3D relative_up = aiVector3D(0.0f, 1.0f, 0.0f);

		//base vectors expressed in relation to this transform base
		const static aiVector3D local_forward;
		const static aiVector3D local_left;
		const static aiVector3D local_up;

		//local translation, rotation, scale
		void Translate(float local_x, float local_y, float local_z);
		void Rotate(float x, float y, float z);
		void Scale(float x, float y, float z);
		void ResetPosition(float rel_x, float rel_y, float rel_z);
	};

	GameObject(const std::string &name, aiNode *node = nullptr);
	virtual ~GameObject();

	void Update(float dt);
	void UpdateWorldTransform();

	Component *CreatePrimitiveMeshComp(ComponentMaterial *mat, float *vertices, float *normals, float *uv);
	Component *CreatePrimitiveMatComp(float *ambient, float *diffuse, float *specular, float shininess);

	Component* CreateLoadedMeshComp(ComponentMaterial *mat, aiMesh *mesh);
	Component* CreateLoadedMaterialComp(aiMesh *mesh, const aiScene *scene, const char *file_name);
	Component* CreateMeshRenderer(ComponentMesh *mesh);
	Component* CreateBehaviour(const std::string &behav_name);
	Component* CreateTorsoBehaviour(const std::string &behav_name);
	Component* CreateAmbientLight();
	Component* CreateDirectionalLight();
	Component* CreatePointLight();

	Transform transform;
	bool dirty = true;
	GameObject *parent_go = nullptr;
	bool active = true;
	std::string name;
	std::vector<Component*> components;
	std::vector<GameObject*> children_go;

	bool game_object_selected = true;

	

private:

	void LoadTransform(aiNode *node);
	void CombineTransform(GameObject *parent_go);
	void UpdateBaseVectors(aiQuaternion rot);
	Gizmo gizmo;
};



#endif
