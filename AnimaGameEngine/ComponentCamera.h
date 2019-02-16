#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Component.h"
#include "Globals.h"
#include <glm/glm.hpp>
#include "Point.h"

class ComponentCamera: public Component
{
public:
	ComponentCamera(ComponentType type, GameObject *ownerGO);
	virtual ~ComponentCamera();

	 virtual void Update(float dt);
	
	 void OnResize(uint width, uint height);

	 const glm::mat4& GetProjectionMatrix() const;
	 const glm::mat4& GetViewMatrix() const;

protected:
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 10.0f);
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
	float cameraSpeedSlow = 5.0f;
	float cameraSpeedFast = 15.0f;
	float cameraWheelSpeed = 30.0f;
	float sensitivity = 10.0f;

	 void UpdateProjectionMatrix();
	 void UpdateViewMatrix();
	 void SetOrientation();
	 void SetViewport(unsigned width, unsigned height);
};
#endif