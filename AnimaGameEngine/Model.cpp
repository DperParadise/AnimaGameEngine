#include "Importer.h"
#include "Model.h"
#include <assimp/scene.h>
#include "Globals.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Mesh.h"
#include "Shader.h"
#include "ComponentMeshRenderer.h"
#include "ComponentCamera.h"


Model::Model(const std::string &filePath, unsigned int load_flags)
{
	Load(filePath, load_flags);
}

Model::~Model() {}

void Model::Load(const std::string &filePath, unsigned int flags)
{
	//flags |= aiPostProcessSteps::aiProcess_PreTransformVertices;
	//flags |= aiPostProcessSteps::aiProcess_FlipUVs;
	//flags |= aiPostProcessSteps::aiProcess_Triangulate;
	//flags |= aiProcessPreset_TargetRealtime_Fast;

	scene = importer->GetInstance()->ReadFile(filePath, flags);
	
	aiNode *root_node = scene->mRootNode;
	modelGO = LoadHierarchy(root_node, nullptr, filePath);

	App->scene->AddGameObject(modelGO);
}

GameObject *Model::LoadHierarchy(aiNode *node, GameObject *parentGO, const std::string &filePath)
{	
	std::string gameObjectName = std::string(node->mName.data);
	if (gameObjectName.empty())
		gameObjectName.append("GameObject");

	GameObject *go = new GameObject(gameObjectName, node);
	go->SetParentGO(parentGO);

	// Create components
	for (uint i = 0; i < node->mNumMeshes; i++)
	{
		uint meshIndex = node->mMeshes[i];
		aiMesh *aiMesh = scene->mMeshes[meshIndex];

		Mesh *mesh = new Mesh();
		unsigned int slashPos = filePath.find_last_of("/");
		std::string texturePath = std::string(filePath, 0, slashPos + 1);
		mesh->LoadMesh(scene, aiMesh, texturePath);

		Shader *shader = new Shader("shaders/vertex.vert", "shaders/fragment.frag"); //TODO: Implement a way to manage shaders

		ComponentCamera *camera = (ComponentCamera*)App->scene->activeCameraComponent;
		go->AddMeshRenderer(mesh, shader, camera);
	}

	for (uint i = 0; i < node->mNumChildren; i++)
	{
		GameObject *childGO = LoadHierarchy(node->mChildren[i], go, filePath);
		go->AddChildGO(childGO);
	}

	return go;
}
