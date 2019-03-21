#include "ModelLoader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "Globals.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Mesh.h"
#include "Shader.h"
#include "ComponentMeshRenderer.h"
#include "ComponentCamera.h"
#include "ComponentTransform.h"

const aiScene *ModelLoader::scene = nullptr;
GameObject *ModelLoader::modelGO = nullptr;

ModelLoader::ModelLoader(){}

ModelLoader::~ModelLoader() {}

 GameObject* ModelLoader::Load(const std::string &filePath, unsigned int flags)
{
	//flags |= aiPostProcessSteps::aiProcess_PreTransformVertices;
	//flags |= aiPostProcessSteps::aiProcess_FlipUVs;
	//flags |= aiPostProcessSteps::aiProcess_Triangulate;
	//flags |= aiProcessPreset_TargetRealtime_Fast;

	Assimp::Importer importer;
	scene = importer.ReadFile(filePath, flags);
	aiNode *root_node = scene->mRootNode;
	modelGO = LoadHierarchy(root_node, nullptr, filePath);

	return modelGO;
}

 GameObject * ModelLoader::LoadMD5(const std::string & filePath, unsigned int flags)
 {
	 Assimp::Importer importer;
	 scene = importer.ReadFile(filePath, flags);
	 aiNode *root_node = scene->mRootNode;
	 modelGO = LoadMeshesMD5(root_node, filePath);

	 return modelGO;
 }

GameObject *ModelLoader::LoadHierarchy(aiNode *node, GameObject *parentGO, const std::string &filePath)
{	
	std::string gameObjectName = std::string(node->mName.data);
	if (gameObjectName.empty())
		gameObjectName.append("GameObject");

	GameObject *go = new GameObject(gameObjectName, node);
	go->SetParentGO(parentGO);

	for (uint i = 0; i < node->mNumMeshes; i++)
	{
		///Load mesh and add a renderer
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

GameObject* ModelLoader::LoadMeshesMD5(aiNode *node, const std::string &filePath)
{
	unsigned int slashPos = filePath.find_last_of("/");
	std::string modelName = std::string(filePath, slashPos + 1);

	aiNode *meshNode = node->FindNode("<MD5_Mesh>");
	if (!node)
	{
		MYLOG("<MD5_Mesh> node not found")
		return nullptr;
	}

	GameObject *go = new GameObject(modelName, meshNode);
	go->SetParentGO(nullptr);

	for (uint i = 0; i < meshNode->mNumMeshes; i++)
	{
		GameObject *meshGO = new GameObject("GameObject" + std::to_string(i), meshNode);
		meshGO->SetParentGO(go);
		go->AddChildGO(meshGO);

		///Load mesh and add a renderer
		uint meshIndex = meshNode->mMeshes[i];
		aiMesh *aiMesh = scene->mMeshes[meshIndex];
		Mesh *mesh = new Mesh();
		unsigned int slashPos = filePath.find_last_of("/");
		std::string texturePath = std::string(filePath, 0, slashPos + 1);
		mesh->LoadMesh(scene, aiMesh, texturePath);

		Shader *shader = new Shader("shaders/vertex.vert", "shaders/fragment.frag"); //TODO: Implement a way to manage shaders

		ComponentCamera *camera = (ComponentCamera*)App->scene->activeCameraComponent;
		meshGO->AddMeshRenderer(mesh, shader, camera);

		///Load bones associated to this mesh
		if (aiMesh->HasBones())
		{
			for (uint j = 0; j < aiMesh->mNumBones; ++j)
			{
				mesh->AddBoneName(aiMesh->mBones[j]->mName.C_Str());

				MYLOG("Mesh: %d bone: %s", i, aiMesh->mBones[j]->mName.C_Str())
			}
		}
	}

	return go;
}
