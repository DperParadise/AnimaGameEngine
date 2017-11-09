#include "ComponentTransform.h"
#include "libraries/assimp/include/assimp/scene.h"
#include "libraries/glew-2.0.0/include/GL/glew.h"

ComponentTransform::ComponentTransform(component_type t, bool act, GameObject *go, aiNode *node) : Component(t, act, go)
{
	local_position.x = 0.0f;
	local_position.y = 0.0f;
	local_position.z = 0.0f;

	local_scale.x = 1.0f;
	local_scale.y = 1.0f;
	local_scale.z = 1.0f;

	local_rotation.x = 0.0f;
	local_rotation.y = 0.0f;
	local_rotation.z = 0.0f;
	local_rotation.w = 1.0f;

	Load(node);
}

ComponentTransform::~ComponentTransform() {}

void ComponentTransform::Load(aiNode *node)
{
	node->mTransformation.Decompose(local_scale, local_rotation, local_position);
}

void ComponentTransform::ComputeGlobalTransform()
{

}

void ComponentTransform::Update(){}

void ComponentTransform::Enable() {}

void ComponentTransform::Disable() {}
