#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Component.h"
#include "Globals.h"
#include "libraries/glm/glm.hpp"

class Config;

class ComponentCamera: public Component
{
public:
	ComponentCamera(ComponentType type, const std::string &name, bool act, GameObject *ownerGO);
	virtual ~ComponentCamera();

	virtual void Update(float dt) override;
	
	virtual void OnResize(uint width, uint height);

	virtual const glm::mat4& GetProjectionMatrix() const;
	virtual const glm::mat4& GetViewMatrix() const;

private:
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::vec3 position = glm::vec3(0.0f, 10.0f, 10.0f);
	float pitch = 0.0f;
	float yaw = -90.0f;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;

	float nearPlane = 0.1f;
	float farPlane = 100.0f;
	float verticalFOV = 70.0f;
	int width = 0.0f;
	int height = 0.0f;
	float aspectRatio = 0.0f;
	float cameraSpeedSlow = 30.0f;
	float cameraSpeedFast = 60.0f;
	float cameraWheelSpeed = 30.0f;
	float sensitivity = 20.0f;

	virtual void UpdateProjectionMatrix() = 0;
	virtual void UpdateViewMatrix() = 0;
	virtual void SetOrientation() = 0;
};
#endif