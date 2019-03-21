#ifndef __SKELETON_H__
#define __SKELETON_H__

#include <glm/gtc/quaternion.hpp>
#include <string>
#include <vector>
#include "Shader.h"

class Mesh;
struct aiScene;
struct aiNode;
class GameObject;

struct BonePose
{
	BonePose();
	glm::quat rotation;
	glm::vec3 position;
	glm::vec3 scale;
};

struct VertexWeight
{
	unsigned int vertexId;
	float weight;
};

class Bone
{
public:
	Bone();
	~Bone();

	BonePose inverseBindPose;
	BonePose worldPose;
	BonePose localPose;
	Bone *parent = nullptr;
	std::vector<Mesh*> meshes; //No hace falta,en principio
	std::vector<Bone*> children;
	std::vector<VertexWeight> verticesWeights;
	std::string name;
	unsigned int id = 0;

private:
	void Clear();
};

class Skeleton
{
public:
	Skeleton(const std::string &skeletonName, const std::string &skeletonPath, Shader *sklShader, GameObject *GOHierarchyRoot);
	~Skeleton();
	void Load(const std::string &skeletonPath);
	void LoadMD5(const std::string &skeletonPath);
	void Update();
	static bool Less(const Bone &a, const Bone &b);
	void Clear();

	std::vector<Bone> skeleton;
	std::vector<Bone> skeletonWorldPoses;
	Bone *skeletonTree = nullptr;
	std::string name;

private:
	void LoadBonesMD5(aiNode *node);
	Bone* LoadSkeletonTreeMD5(aiNode *node, Bone *parentBone);
	void PrintSkeleton() const;
	void PrintSkeletonTree(Bone *bone, int spaces) const;
	void BindBoneToMesh(GameObject *goHierarchy);
	int FindBoneInSkeleton(const std::string &nodeName) const;
	Bone* FindBoneInSkeletonTree(const std::string &boneName, Bone *bone);
	void UpdateSkeletonTreePoses(Bone *bone);

	std::vector<glm::vec3> skeletonVertices;
	unsigned int VAO, VBO, EBO;
	Shader *shader = nullptr;
	const aiScene *scene = nullptr;
	GameObject *GOHierarchy = nullptr;
};
#endif

