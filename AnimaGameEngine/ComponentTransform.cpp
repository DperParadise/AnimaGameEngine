#include "ComponentTransform.h"
#include "libraries/assimp/include/assimp/scene.h"
#include "libraries/glew-2.0.0/include/GL/glew.h"

ComponentTransform::ComponentTransform(component_type t, bool act, GameObject *go, aiNode *node) : Component(t, act, go)
{
	local_position.Set(0.0f, 0.0f, 0.0f);

	local_scale.Set(0.0f, 0.0f, 0.0f);

	local_rotation.x = 0.0f;
	local_rotation.y = 0.0f;
	local_rotation.z = 0.0f;
	local_rotation.w = 1.0f;

	world_position.Set(0.0f, 0.0f, 0.0f);

	world_scale.Set(0.0f, 0.0f, 0.0f);

	world_rotation.x = 0.0f;
	world_rotation.y = 0.0f;
	world_rotation.z = 0.0f;
	world_rotation.w = 1.0f;

	Load(node);
}

ComponentTransform::~ComponentTransform() {}

void ComponentTransform::Load(aiNode *node)
{
	node->mTransformation.Decompose(local_scale, local_rotation, local_position);
}

void ComponentTransform::Update(float dt){}

void ComponentTransform::Enable() {}

void ComponentTransform::Disable() {}
