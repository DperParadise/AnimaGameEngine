#include "Skeleton.h"
#include "Globals.h"

Skeleton::Skeleton() {}
Skeleton::~Skeleton() 
{
	for (SkeletonTreeNode &node : skeletonTree)
	{
		RELEASE_ARRAY(node.jointIds)
	}
}

void Skeleton::Draw() const {}

bool Skeleton::Less(const Joint & a, const Joint & b)
{
	 return a.id < b.id; 
}
