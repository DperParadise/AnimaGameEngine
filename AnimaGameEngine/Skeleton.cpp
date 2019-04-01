#include "Skeleton.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "Globals.h"
#include "GameObject.h"
#include "ComponentMeshRenderer.h"
#include "Mesh.h"
#include <map>
#include "ComponentMeshRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glew_2.0.0/GL/glew.h>
#include "ComponentCamera.h"

Skeleton::Skeleton(const std::string &skeletonName, 
	const std::string &skeletonPath, 
	Shader *skeletonShader,
	const ComponentCamera *camera) : name(skeletonName), skeletonShader(skeletonShader), camera(camera)
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

	//begin test
	aiMatrix4x4 aiInvRootNode = rootNode->mTransformation;
	aiInvRootNode.Inverse();

	auto pRow1 = aiInvRootNode[0];
	auto pRow2 = aiInvRootNode[1];
	auto pRow3 = aiInvRootNode[2];
	auto pRow4 = aiInvRootNode[3];
	
	glm::vec4 col1(*pRow1, *(pRow1 + 1), *(pRow1 + 2), *(pRow1 + 3));
	glm::vec4 col2(*pRow2, *(pRow2 + 1), *(pRow2 + 2), *(pRow2 + 3));
	glm::vec4 col3(*pRow3, *(pRow3 + 1), *(pRow3 + 2), *(pRow3 + 3));
	glm::vec4 col4(*pRow4, *(pRow4 + 1), *(pRow4 + 2), *(pRow4 + 3));

	invRootNode = glm::mat4(col1, col2, col3, col4);
	//end test


	LoadBonesMD5(rootNode->FindNode("<MD5_Mesh>"));
	PrintSkeleton();

	SetInvBindPoseMatrices();
	SetSkeletonPosesMatrices();

	skeletonTree = LoadSkeletonTreeMD5(rootNode->FindNode("origin"), nullptr);
	PrintSkeletonTree(skeletonTree, 1);
	PrintSkeleton();
	
	//To draw the skeleton - order matters!
	SetSkeletonToDraw();
	SetSkeletonVertexBuffers();
}

void Skeleton::Update()
{
	UpdateSkeletonTreePoses(skeletonTree);
	
	DrawSkeleton(camera->GetViewMatrix(), camera->GetProjectionMatrix());
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

				bone.name = std::string(aiBone->mName.C_Str());
				bone.id = boneId;
				skeleton.push_back(bone);
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

	//Set skeleton to bind pose
	aiMatrix4x4 transform = node->mTransformation;
	aiVector3D scaling;
	aiQuaternion rot;
	aiVector3D position;
	transform.Decompose(scaling, rot, position);

	newBone->localPose.scale.x = scaling.x;
	newBone->localPose.scale.y = scaling.y;
	newBone->localPose.scale.z = scaling.z;

	newBone->localPose.rotation.x = rot.x;
	newBone->localPose.rotation.y = rot.y;
	newBone->localPose.rotation.z = rot.z;
	newBone->localPose.rotation.w = rot.w;

	newBone->localPose.position.x = position.x;
	newBone->localPose.position.y = position.y;
	newBone->localPose.position.z = position.z;


	//Bones in the hierarchy that are not related to meshes but still affect the hierarchy, i.e: bone "origin"
	if(FindBoneInSkeleton(newBone->name) == -1)
	{
		skeleton.push_back(*newBone);
	}

	//update skelton with bind pose
	int boneIndex = FindBoneInSkeleton(newBone->name);
	skeleton[boneIndex].localPose = newBone->localPose;

	for (uint i = 0; i < node->mNumChildren; ++i)
	{
		Bone *childBone = LoadSkeletonTreeMD5(node->mChildren[i], newBone);
		newBone->children.push_back(childBone);
	}

	return newBone;
}

void Skeleton::PrintSkeleton() const
{
	MYLOG("============= BEGIN BONES ===============")
	for (const Bone &b : skeleton)
	{
		MYLOG("bone: %s", b.name.c_str())
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
	int boneIndex = FindBoneInSkeleton(bone->name);
	if (boneIndex != -1)
	{
		//Update bone's local pose
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

		//Update poses matrices
		glm::mat4 pose;

		//begin test
		//pose = pose * invRootNode;
		//end test

		
		pose = glm::translate(pose, bone->worldPose.position);
		pose = pose * glm::mat4_cast(bone->worldPose.rotation);
		pose = glm::scale(pose, bone->worldPose.scale);
		
		pose = pose * invBindPoseMatrices[boneIndex];
		
		skeletonPoses[boneIndex] = pose;
	}
	for (Bone *childBone : bone->children)
	{
		UpdateSkeletonTreePoses(childBone);
	}
}

void Skeleton::SetInvBindPoseMatrices()
{
	invBindPoseMatrices.reserve(skeleton.size());

	for (const Bone &bone : skeleton)
	{
		glm::mat4 invBindMatrix;
		invBindMatrix = glm::translate(invBindMatrix, bone.inverseBindPose.position);
		invBindMatrix = invBindMatrix * glm::mat4_cast(bone.inverseBindPose.rotation);
		invBindMatrix = glm::scale(invBindMatrix, bone.inverseBindPose.scale);
		invBindPoseMatrices.push_back(invBindMatrix);
	}
}

void Skeleton::SetSkeletonPosesMatrices()
{
	skeletonPoses.resize(skeleton.size());
}

void Skeleton::SetSkeletonVertexBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, boneHierarchy.size() * sizeof(glm::vec3), NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glEnable(GL_PROGRAM_POINT_SIZE);
}

void Skeleton::SetSkeletonToDraw()
{
	//setup the endBones vector
	for (auto it = skeleton.begin(); it != skeleton.end(); ++it)
	{
		Bone *bone = FindBoneInSkeletonTree((*it).name, skeletonTree);
		if (!bone)
		{
			MYLOG("Bone \"%s\" not found in skeletonTree", bone->name);
			return;
		}
		endBones.push_back(bone);
	}
	
	//count the ancestors for each end bone
	unsigned int maxSize = 0;
	for (const Bone *bone : endBones)
	{
		unsigned int size = CountBonesInHierarchy(bone);
		endBonesHierachySize.push_back(size);
		
		if (size > maxSize)
			maxSize = size;
	}
	
	//resize the bones positions vector to be used as input to the shader
	boneHierarchy.resize(maxSize);
}

unsigned int Skeleton::CountBonesInHierarchy(const Bone *bone)
{
	static unsigned int numBones = 0;
	if (!bone)
	{
		unsigned int ret = numBones;
		numBones = 0;
		return ret;
	}

	++numBones;
	CountBonesInHierarchy(bone->parent);
}

void Skeleton::UpdateSkeletonToDraw(const Bone *bone)
{
	static unsigned int index = 0;
	if (!bone)
	{
		index = 0;
		return;
	}
	boneHierarchy[index] = bone->worldPose.position;
	++index;
	UpdateSkeletonToDraw(bone->parent);
}

void Skeleton::DrawSkeleton(const glm::mat4 &view, const glm::mat4 &projection)
{
	skeletonShader->Use();
	skeletonShader->SetMat4("view", view);
	skeletonShader->SetMat4("projection", projection);

	for (unsigned int i = 0; i < endBones.size(); ++i)
	{
		UpdateSkeletonToDraw(endBones[i]);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, endBonesHierachySize[i] * sizeof(glm::vec3), &boneHierarchy[0], GL_DYNAMIC_DRAW);
		glDrawArrays(GL_LINES, 0, endBonesHierachySize[i]);
		glDrawArrays(GL_POINTS, 0, endBonesHierachySize[i]);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

bool Skeleton::Less(const Bone &left, const Bone &right)
{
	return left.id < right.id;
}

void Skeleton::Clear()
{
	RELEASE(skeletonTree);
	RELEASE(skeletonShader);
}

BonePose::BonePose() :scale(glm::vec3(1.0f)) {}

Bone::Bone(){}

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


