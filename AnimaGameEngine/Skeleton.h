#ifndef __SKELETON_H__
#define __SKELETON_H__

#include <glm/gtc/quaternion.hpp>
#include <string>
#include <vector>
#include "Globals.h"

class Mesh;
class ComponentTransform;
class GameObject;

struct JointPose
{
	glm::quat rotation;
	glm::vec3 position;
	glm::vec3 scale;
};

struct VertexWeight
{
	unsigned int vertexId;
	float weight;
};

struct Joint
{
	JointPose inverseBindPose;
	JointPose localPose;
	Mesh *mesh;
	std::vector<VertexWeight> verticesWeights;
	std::string name;
	unsigned int id;
};

struct SkeletonTreeNode
{
	GameObject *ownerGO = nullptr;
	unsigned int *jointIds = nullptr;
};

class Skeleton
{
public:
	Skeleton();
	~Skeleton();

	std::vector<Joint> skeleton;
	std::vector<SkeletonTreeNode> skeletonTree;

	void Draw() const;
	static bool Less(const Joint &a, const Joint &b);
};
#endif

