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
#include "Skeleton.h"
#include <glm/glm.hpp>
#include <algorithm>
#include "ComponentTransform.h"

Model::Model(const std::string &filePath, unsigned int load_flags)
{
	Load(filePath, load_flags);
}

Model::~Model() {}

Skeleton& Model::GetSkeleton()
{
	return modelSkeleton;
}

void Model::Load(const std::string &filePath, unsigned int flags)
{
	//flags |= aiPostProcessSteps::aiProcess_PreTransformVertices;
	//flags |= aiPostProcessSteps::aiProcess_FlipUVs;
	//flags |= aiPostProcessSteps::aiProcess_Triangulate;
	//flags |= aiProcessPreset_TargetRealtime_Fast;

	scene = importer->GetInstance()->ReadFile(filePath, flags);
	
	aiNode *root_node = scene->mRootNode;
	modelGO = LoadHierarchy(root_node, nullptr, filePath);

	std::sort(modelSkeleton.skeleton.begin(), modelSkeleton.skeleton.end(), &Skeleton::Less);

	App->scene->AddGameObject(modelGO);
}

GameObject *Model::LoadHierarchy(aiNode *node, GameObject *parentGO, const std::string &filePath)
{	
	std::string gameObjectName = std::string(node->mName.data);
	if (gameObjectName.empty())
		gameObjectName.append("GameObject");

	GameObject *go = new GameObject(gameObjectName, node);
	go->SetParentGO(parentGO);

	SkeletonTreeNode skeletonNode;
	skeletonNode.ownerGO = go;

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

		///Load skeleton
		if (aiMesh->HasBones())
		{
			skeletonNode.jointIds = new unsigned int[aiMesh->mNumBones];

			static unsigned int jointId = 0;
			for (int i = 0; i < aiMesh->mNumBones; ++i)
			{
				skeletonNode.jointIds[i] = jointId;
				modelSkeleton.skeletonTree.push_back(skeletonNode);

				aiMatrix4x4 inverseBindPose = aiMesh->mBones[i]->mOffsetMatrix;
				
				aiVector3D pos, scale;
				aiQuaternion rot;

				inverseBindPose.Decompose(scale, rot, pos);

				glm::vec3 posGLM(pos.x, pos.y, pos.z);
				glm::vec3 scaleGLM(scale.x, scale.y, scale.z);
				glm::quat rotGLM(rot.w, rot.x, rot.y, rot.z);
			
				Joint joint;
				joint.inverseBindPose.rotation = rotGLM;
				joint.inverseBindPose.position = posGLM;
				joint.inverseBindPose.scale = scaleGLM;

				joint.mesh = mesh;

				joint.verticesWeights.reserve(aiMesh->mBones[i]->mNumWeights);
				memcpy(joint.verticesWeights.data(), aiMesh->mBones[i]->mWeights, aiMesh->mBones[i]->mNumWeights * sizeof(VertexWeight));
				
				joint.name = std::string(aiMesh->mBones[i]->mName.C_Str());
				
				joint.id = jointId++;
				
				modelSkeleton.skeleton.push_back(joint);	
			}
		}
	}

	for (uint i = 0; i < node->mNumChildren; i++)
	{
		GameObject *childGO = LoadHierarchy(node->mChildren[i], go, filePath);
		go->AddChildGO(childGO);
	}

	return go;
}
