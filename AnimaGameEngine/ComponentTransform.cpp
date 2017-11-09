#include "ComponentTransform.h"
#include "libraries/assimp/include/assimp/scene.h"
#include "libraries/glew-2.0.0/include/GL/glew.h"

ComponentTransform::ComponentTransform(component_type t, bool act, GameObject *go, aiNode *node) : Component(t, act, go)
{
	position.x = 0.0f;
	position.y = 0.0f;
	position.z = 0.0f;

	scale.x = 1.0f;
	scale.y = 1.0f;
	scale.z = 1.0f;

	rotation.x = 0.0f;
	rotation.y = 0.0f;
	rotation.z = 0.0f;
	rotation.w = 0.0f;

	Load(node);
}

ComponentTransform::~ComponentTransform() {}

void ComponentTransform::Load(aiNode *node)
{
	node->mTransformation.Decompose(scale, rotation, position);
}

void ComponentTransform::Update(){}

void ComponentTransform::Enable() {}

void ComponentTransform::Disable() {}
