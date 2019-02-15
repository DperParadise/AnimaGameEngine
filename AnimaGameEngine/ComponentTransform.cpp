#include "ComponentTransform.h"
//#include <glm/gtx/rotate_vector.hpp> //TODO: what for?
#include "GameObject.h"

//define Transform static fields
const glm::vec3 ComponentTransform::worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 ComponentTransform::worldForward = glm::vec3(0.0f, 0.0f, 1.0f);
const glm::vec3 ComponentTransform::worldLeft = glm::vec3(1.0f, 0.0f, 0.0f);

ComponentTransform::ComponentTransform(ComponentType type, GameObject *ownerGO) : 
	Component(type, ownerGO){

	relativePosition = glm::vec3(0.0f);
	relativeScale = glm::vec3(1.0f);
	relativeRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // Note that: quat(w, x, y, z)

	worldPosition = glm::vec3(0.0f);
	worldScale = glm::vec3(1.0f);
	worldRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
}

ComponentTransform::~ComponentTransform() {}

void ComponentTransform::Translate(const glm::vec3 & translation)
{
	worldPosition = translation;

	//Since worldPosition = relativePos + parentWorldPos. We need to calculate the new relativePos
	if (GetOwnerGO()->GetParentGO())
	{
		glm::vec3 parentWorldPos = GetOwnerGO()->GetParentGO()->GetTransform()->GetWorldPosition();
		glm::vec3 relPos = worldPosition - parentWorldPos;
		relativePosition = relPos;
	}
	else
	{
		relativePosition = worldPosition;
	}
}

void ComponentTransform::Rotate(const glm::vec3 & eulerAnglesInDegrees)
{
	worldRotation = glm::quat(glm::radians(eulerAnglesInDegrees));

	//Since worlRotation = parentWorlRot * relativeRot. We need to calculate the new relativeRot
	//Rw_child = Rw_parent * Rr_child; inv(Rw_parent) * Rw_child = Rr_child;
	if (GetOwnerGO()->GetParentGO())
	{
		glm::quat parentWorldRot = GetOwnerGO()->GetParentGO()->GetTransform()->GetWorldRotation();
		// inverse == conjugate in rotations
		glm::quat inverseParentWorldRot = glm::conjugate(parentWorldRot); 
		relativeRotation = inverseParentWorldRot * worldRotation;
	}
	else
	{
		relativeRotation = worldRotation;	
	}

	//TODO: Check rotation order => rotation*vector or vector*rotation
	//ownForward = worldRotation * ownForward;
	//ownUp = worldRotation * ownUp;
	//ownLeft = worldRotation * ownLeft;
}

void ComponentTransform::Scale(const glm::vec3 & scale)
{
	worldScale = scale;

	if (GetOwnerGO()->GetParentGO())
	{
		glm::vec3 parentWorldScale = GetOwnerGO()->GetParentGO()->GetTransform()->GetWorldScale();
		relativeScale = parentWorldScale / worldScale;
	}
	else
	{
		relativeScale = worldScale;
	}
}

void ComponentTransform::ResetPosition()
{
	// TODO: What to reset?
}

const glm::vec3 & ComponentTransform::GetRelativePositionWorldAxis() const
{
	return relativePosition;
}

const glm::vec3 & ComponentTransform::GetRelativeScaleWorldAxis() const
{
	return relativeScale;
}

const glm::quat & ComponentTransform::GetRelativeRotationWorldAxis() const
{
	return relativeRotation;
}

const glm::vec3 & ComponentTransform::GetWorldPosition() const
{
	return worldPosition;
}

const glm::vec3 & ComponentTransform::GetWorldScale() const
{
	return worldScale;
}

const glm::quat & ComponentTransform::GetWorldRotation() const
{
	return worldRotation;
}

void ComponentTransform::SetRelativePositionWorldAxis(const glm::vec3 & pos)
{
	relativePosition = pos;
}

void ComponentTransform::SetRelativeScaleWorldAxis(const glm::vec3 & scale)
{
	relativeScale = scale;
}

void ComponentTransform::SetRelativeRotationWorldAxis(const glm::quat & rot)
{
	relativeRotation = rot;
}

void ComponentTransform::SetWorldPosition(const glm::vec3 & pos)
{
	worldPosition = pos;
}

void ComponentTransform::SetWorldScale(const glm::vec3 & scale)
{
	worldScale = scale;
}

void ComponentTransform::SetWorldRotation(const glm::quat & rot)
{
	worldRotation = rot;
}
