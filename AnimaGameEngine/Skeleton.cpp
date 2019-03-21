#include "Skeleton.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "Globals.h"
#include "GameObject.h"
#include "ComponentMeshRenderer.h"
#include "Mesh.h"

Skeleton::Skeleton(const std::string &skeletonName, 
	const std::string &skeletonPath, 
	Shader *sklShader,
	GameObject *GOHierarchyRoot) : name(skeletonName), shader(sklShader), GOHierarchy(GOHierarchyRoot)
{
	LoadMD5(skeletonPath);
}
Skeleton::~Skeleton() 
{
	Clear();
}

void Skeleton::Load(const std::string &skeletonPath)
{
	Assimp::Importer importer;
	scene = importer.ReadFile(skeletonPath, 0);
	if (!scene)
	{
		MYLOG("Error importing skeleton")
		return;
	}
	aiNode *rootNode = scene->mRootNode;
	//LoadSkeleton(rootNode);
	
}

void Skeleton::LoadMD5(const std::string & skeletonPath)
{
	Assimp::Importer importer;
	scene = importer.ReadFile(skeletonPath, 0);
	if (!scene)
	{
		MYLOG("Error importing skeleton")
		return;
	}
	aiNode *rootNode = scene->mRootNode;
	LoadBonesMD5(rootNode->FindNode("<MD5_Mesh>"));
	PrintSkeleton();

	skeletonTree = LoadSkeletonTreeMD5(rootNode->FindNode("<MD5_Hierarchy>"), nullptr);
	PrintSkeletonTree(skeletonTree, 1);

}

void Skeleton::Update()
{
	UpdateSkeletonTreePoses(skeletonTree);
}

void Skeleton::LoadBonesMD5(aiNode* node)
{
	for (uint i = 0; i < node->mNumMeshes; ++i)
	{
		uint meshIndex = node->mMeshes[i];
		aiMesh *aiMesh = scene->mMeshes[meshIndex];
		if (aiMesh->HasBones())
		{
			for (uint j = 0; j < aiMesh->mNumBones; ++j)
			{
				static unsigned int boneId = 0;

				aiBone *aiBone = aiMesh->mBones[j];

				//Check If bone is already in the skeleton
				if (FindBoneInSkeleton(aiBone->mName.C_Str()) != -1)
				{
					continue;
				}

				aiMatrix4x4 offsetMatrix = aiBone->mOffsetMatrix;

				Bone bone;
				aiVector3D scaling;
				aiQuaternion rot;
				aiVector3D pos;
				offsetMatrix.Decompose(scaling, rot, pos);

				bone.inverseBindPose.scale.x = scaling.x;
				bone.inverseBindPose.scale.y = scaling.y;
				bone.inverseBindPose.scale.z = scaling.z;

				bone.inverseBindPose.rotation.x = rot.x;
				bone.inverseBindPose.rotation.y = rot.y;
				bone.inverseBindPose.rotation.z = rot.z;
				bone.inverseBindPose.rotation.w = rot.w;

				bone.inverseBindPose.position.x = pos.x;
				bone.inverseBindPose.position.y = pos.y;
				bone.inverseBindPose.position.z = pos.z;

				aiVertexWeight *vertexWeights = aiBone->mWeights;
				for (uint k = 0; k < aiBone->mNumWeights; ++k)
				{
					VertexWeight vw;
					vw.vertexId = vertexWeights->mVertexId;
					vw.weight = vertexWeights->mWeight;
					bone.verticesWeights.push_back(vw);
				}

				bone.name = std::string(aiBone->mName.C_Str());
				bone.id = boneId;
				skeleton.push_back(bone);
				skeletonWorldPoses.push_back(bone);
				++boneId;
			}
		}
	}
}

Bone* Skeleton::LoadSkeletonTreeMD5(aiNode *node, Bone *parentBone)
{
	Bone *newBone = new Bone();
	newBone->name = node->mName.C_Str();
	newBone->parent = parentBone;

	//Initialize the skeleton vector with poses taken from the skeleton tree
	aiMatrix4x4 transform = node->mTransformation;
	aiVector3D scaling;
	aiQuaternion rot;
	aiVector3D position;
	transform.Decompose(scaling, rot, position);

	int index = FindBoneInSkeleton(newBone->name);
	if (index != -1)
	{
		skeleton[index].localPose.scale.x = scaling.x;
		skeleton[index].localPose.scale.y = scaling.y;
		skeleton[index].localPose.scale.z = scaling.z;

		skeleton[index].localPose.rotation.x = rot.x;
		skeleton[index].localPose.rotation.y = rot.y;
		skeleton[index].localPose.rotation.z = rot.z;
		skeleton[index].localPose.rotation.w = rot.w;

		skeleton[index].localPose.position.x = position.x;
		skeleton[index].localPose.position.y = position.y;
		skeleton[index].localPose.position.z = position.z;
	}
	if (parentBone)
		parentBone->children.push_back(newBone);

	for (uint i = 0; i < node->mNumChildren; ++i)
	{
		LoadSkeletonTreeMD5(node->mChildren[i], newBone);
	}

	return newBone;
}

void Skeleton::PrintSkeleton() const
{
	MYLOG("============= BEGIN BONES ===============")
	for (const Bone &b : skeleton)
	{
		MYLOG("bone: %s Meshes bound=%d", b.name.c_str(), b.meshes.size())
	}
	MYLOG("============= END BONES =================")
}

void Skeleton::PrintSkeletonTree(Bone *bone, int spaces) const
{
	int numSpaces = spaces;
	std::string spacesString(numSpaces, ' ');
	std::string boneName(bone->name);
	boneName.insert(0, spacesString);
	MYLOG("bone: %s", boneName.c_str())
	++numSpaces;
	for (Bone *b : bone->children)
	{
		PrintSkeletonTree(b, numSpaces);
	}
}

//Bind bone to mesh for a MD5 model loaded with assimp - No hace falta, en principio.
void Skeleton::BindBoneToMesh(GameObject *goHierarchy)
{
	for (GameObject *child : goHierarchy->GetChildrenGO())
	{
		ComponentMeshRenderer *mr = (ComponentMeshRenderer*)child->FindComponentByType(ComponentType::MESH_RENDERER);
		Mesh *mesh = mr->GetMesh();
		
		for (const std::string &jointName : mesh->GetBones())
		{
			int nodeId = FindBoneInSkeleton(jointName);
			skeleton[nodeId].meshes.push_back(mesh);
		}
	}
}

int Skeleton::FindBoneInSkeleton(const std::string &nodeName) const
{
	for (const Bone &joint : skeleton) 
	{
		if (nodeName == joint.name)
			return joint.id;
	}
	return -1;
}

Bone* Skeleton::FindBoneInSkeletonTree(const std::string &boneName, Bone *bone)
{
	if (bone->name == boneName)
		return bone;

	for (Bone *child : bone->children)
	{
		Bone *found = FindBoneInSkeletonTree(boneName, child);
		if (found)
			return found;
	}

	return nullptr;
}

//called every frame
void Skeleton::UpdateSkeletonTreePoses(Bone *bone)
{
	//Update bone's local pose
	int boneIndex = FindBoneInSkeleton(bone->name);
	if (boneIndex != -1)
	{
		Bone &foundBone = skeleton[boneIndex];
		bone->localPose = foundBone.localPose;

		//Update bone's world pose
		if (bone->parent)
		{
			bone->worldPose.scale = bone->parent->worldPose.scale * bone->localPose.scale;
			bone->worldPose.rotation = bone->parent->worldPose.rotation * bone->localPose.rotation;
			bone->worldPose.position = bone->parent->worldPose.position + bone->localPose.position;
		}
		else
		{
			bone->worldPose.scale = bone->localPose.scale;
			bone->worldPose.rotation = bone->localPose.rotation;
			bone->worldPose.position = bone->localPose.position;
		}

		//Update vector of world poses
		static int index = 0;
		skeletonWorldPoses[index].worldPose = bone->worldPose;
		++index;
		if (index == skeleton.size())
			index = 0;
	}
	for (Bone *childBone : bone->children)
	{
		UpdateSkeletonTreePoses(childBone);
	}
}

bool Skeleton::Less(const Bone &left, const Bone &right)
{
	return left.id < right.id;
}

void Skeleton::Clear()
{
	RELEASE(skeletonTree)
}

BonePose::BonePose() :scale(glm::vec3(1.0f)) {}

Bone::Bone() {}

Bone::~Bone()
{
	Clear();
}

void Bone::Clear()
{
	for (Bone *child : children)
	{
		RELEASE(child);
	}
	children.clear();
}


