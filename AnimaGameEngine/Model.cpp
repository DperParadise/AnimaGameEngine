#include "Importer.h"
#include "Model.h"
#include "libraries/assimp/include/assimp/scene.h"
#include "libraries/assimp/include/assimp/postprocess.h"
#include "Globals.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleScene.h"

Model::Model(std::string name, const char *file)
{
	Load(name, file);
}

Model::~Model() {}

void Model::Load(std::string name, const char *file)
{
	scene = importer->GetInstance()->ReadFile(file,  /*aiPostProcessSteps::aiProcess_PreTransformVertices |*/ 
		aiPostProcessSteps::aiProcess_FlipUVs |
		aiPostProcessSteps::aiProcess_Triangulate /*| 
	aiProcessPreset_TargetRealtime_Fast*/);
	
	model_go = new GameObject(name);
	model_go->parent_go = nullptr;
	
	aiNode *root_node = scene->mRootNode;
	LoadHierarchy(root_node, model_go, file);

	App->scene->AddGameObject(model_go);
}

void Model::LoadHierarchy(aiNode *node, GameObject *go, const char *file)
{	
	for (uint i = 0; i < node->mNumChildren; i++)
	{
		std::string node_name = std::string(node->mName.data);
		if (node_name.empty())
			node_name.append("child_go");

		GameObject *child_go = new GameObject(node_name, node);
		child_go->parent_go = go;
		go->children_go.push_back(child_go);

		aiNode *child_node = node->mChildren[i];

		LoadHierarchy(child_node, child_go, file);
	}

	//load component mesh and material. Create mesh renderer
	for (uint i = 0; i < node->mNumMeshes; i++)
	{
		uint mesh_index = node->mMeshes[i];
		Component *mat_comp = go->CreateMaterialComp(scene->mMeshes[mesh_index], scene, file);
		Component *mesh_comp = go->CreateMeshComp((ComponentMaterial*)mat_comp, scene->mMeshes[mesh_index]);
		go->CreateMeshRenderer((ComponentLoadedMesh*)mesh_comp);
	}
}
