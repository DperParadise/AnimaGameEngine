#include "ComponentTransform.h"
#include "libraries/glm/gtx/rotate_vector.hpp"

//define Transform static fields
const glm::vec3 ComponentTransform::worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 ComponentTransform::worldForward = glm::vec3(0.0f, 0.0f, 1.0f);
const glm::vec3 ComponentTransform::worldLeft = glm::vec3(1.0f, 0.0f, 0.0f);

ComponentTransform::ComponentTransform(ComponentType type, const std::string &name, GameObject *ownerGO) : 
	Component(type, name, ownerGO){ }

ComponentTransform::~ComponentTransform() {}

void ComponentTransform::Translate(const glm::vec3 & translation)
{
	worldPosition += translation;
}

void ComponentTransform::Rotate(float angle, const glm::vec3 & axis)
{
	worldRotation = glm::rotate(worldRotation, glm::radians(angle), axis);

	//TODO: Check rotation order => rotation*vector or vector*rotation
	relativeForward = worldRotation * relativeForward;
	relativeUp = worldRotation * relativeUp;
	relativeLeft = worldRotation * relativeLeft;
}

void ComponentTransform::Scale(const glm::vec3 & scale)
{
	worldScale *= scale;
}

void ComponentTransform::ResetPosition()
{
	// TODO: What to reset?
}
