#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class ComponentTransform : public Component
{
public:
	ComponentTransform(ComponentType type, GameObject *ownerGO);
	~ComponentTransform();

	
	void Translate(const glm::vec3 &translation);
	void Rotate(const glm::vec3 &eulerAnglesInDegrees);
	void Scale(const glm::vec3 &scale);
	void ResetPosition();

	const glm::vec3& GetRelativePositionWorldAxis() const;
	const glm::vec3& GetRelativeScaleWorldAxis() const;
	const glm::quat& GetRelativeRotationWorldAxis() const;

	const glm::vec3& GetWorldPosition() const;
	const glm::vec3& GetWorldScale() const;
	const glm::quat& GetWorldRotation() const;

	//Set transform on world's axis
	void SetRelativePositionWorldAxis(const glm::vec3& pos);
	void SetRelativeScaleWorldAxis(const glm::vec3& scale);
	void SetRelativeRotationWorldAxis(const glm::quat& rot);

	//On world axis
	void SetWorldPosition(const glm::vec3& pos);
	void SetWorldScale(const glm::vec3& scale);
	void SetWorldRotation(const glm::quat& rot);

private:
	glm::vec3 acumRelPosition;

	///vectors on world axis
	glm::vec3 relativePosition;
	glm::vec3 relativeScale;
	glm::quat relativeRotation;

	///base vectors of this transform
	glm::vec3 ownForward;
	glm::vec3 ownLeft;
	glm::vec3 ownUp;

	glm::vec3 worldPosition;
	glm::vec3 worldScale;
	glm::quat worldRotation;

	static const glm::vec3 worldUp;
	static const glm::vec3 worldForward;
	static const glm::vec3 worldLeft;
};

#endif
