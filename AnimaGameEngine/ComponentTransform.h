#ifndef __COMPONENT_TRANSFORM_H__
#define __COMPONENT_TRANSFORM_H__

#include "Component.h"
#include "libraries/assimp/include/assimp/vector3.h"
#include "libraries/assimp/include/assimp/quaternion.h"

class GameObject;
struct aiNode;
class ComponentTransform : public Component
{

public:
	ComponentTransform(component_type t, bool act, GameObject *go, aiNode *node);
	virtual ~ComponentTransform();

	void Load(aiNode *node);

	virtual void Update(float dt);
	virtual void Enable();
	virtual void Disable();

	aiVector3D local_position;
	aiVector3D local_scale;
	aiQuaternion local_rotation;

	aiVector3D world_position;
	aiVector3D world_scale;
	aiQuaternion world_rotation;


};




#endif
