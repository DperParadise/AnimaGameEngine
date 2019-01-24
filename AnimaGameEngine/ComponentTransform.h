#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "Component.h"
#include "libraries/glm/glm.hpp"
#include "libraries/glm/gtc/quaternion.hpp"

class ComponentTransform : public Component
{
public:
	ComponentTransform(ComponentType type, const std::string &name, GameObject *ownerGO);
	~ComponentTransform();

	
	void Translate(const glm::vec3 &translation);
	void Rotate(float angle, const glm::vec3 &axis);
	void Scale(const glm::vec3 &scale);
	void ResetPosition();

private:
	glm::vec3 acumRelPosition;

	glm::vec3 relativePosition;
	glm::vec3 relativeScale;
	glm::quat relativeRotation;

	glm::vec3 relativeForward;
	glm::vec3 relativeLeft;
	glm::vec3 relativeUp;

	glm::vec3 worldPosition;
	glm::vec3 worldScale;
	glm::quat worldRotation;

	static const glm::vec3 worldUp;
	static const glm::vec3 worldForward;
	static const glm::vec3 worldLeft;
};

#endif
