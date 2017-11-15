#include "Importer.h"
#include "Model.h"
#include "libraries/assimp/include/assimp/scene.h"
#include "libraries/assimp/include/assimp/postprocess.h"
#include "Globals.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleScene.h"

Model::Model(const char *file)
{
	Load(file);
}

Model::~Model() {}

void Model::Load(const char *file)
{
	unsigned flags = 0;
	//flags |= aiPostProcessSteps::aiProcess_PreTransformVertices;
	//flags |= aiPostProcessSteps::aiProcess_FlipUVs;
	flags |= aiPostProcessSteps::aiProcess_Triangulate;
	//flags |= aiProcessPreset_TargetRealtime_Fast;

	scene = importer->GetInstance()->ReadFile(file, flags);
	
	aiNode *root_node = scene->mRootNode;
	model_go = LoadHierarchy(root_node, nullptr, file);

	App->scene->AddGameObject(model_go);
}

GameObject *Model::LoadHierarchy(aiNode *node, GameObject *parent_go, const char *file)
{	
	std::string go_name = std::string(node->mName.data);
	if (go_name.empty())
		go_name.append("GameObject");

	GameObject *go = new GameObject(go_name, node);
	go->parent_go = parent_go;

	// Load/create components
	for (uint i = 0; i < node->mNumMeshes; i++)
	{
		uint mesh_index = node->mMeshes[i];
		Component *mat_comp = go->CreateLoadedMaterialComp(scene->mMeshes[mesh_index], scene, file);
		Component *mesh_comp = go->CreateLoadedMeshComp((ComponentMaterial*)mat_comp, scene->mMeshes[mesh_index]);
		go->CreateMeshRenderer((ComponentMesh*)mesh_comp);
	}

	for (uint i = 0; i < node->mNumChildren; i++)
	{
		GameObject *child_go = LoadHierarchy(node->mChildren[i], go, file);
		go->children_go.push_back(child_go);
	}

	return go;
}
