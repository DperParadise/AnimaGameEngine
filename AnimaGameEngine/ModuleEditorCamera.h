#ifndef __MODULECAMEDITOR_H__
#define __MODULECAMEDITOR_H__

#include "Globals.h"
#include "Module.h"
#include "libraries/glm/glm.hpp"


class Config;
class ModuleEditorCamera : public Module
{
public:
	ModuleEditorCamera();
	virtual ~ModuleEditorCamera();

	bool Init(Config *config) override;
	update_status Update(float dt) override;
	bool CleanUp() override;
	
	void OnResize();

	const glm::mat4& GetProjectionMatrix() const;
	const glm::mat4& GetViewMatrix() const;

private:
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
	int width = 0;
	int height = 0;
	float aspectRatio = 0.0f;
	float cameraSpeedSlow = 30.0f;
	float cameraSpeedFast = 60.0f;
	float cameraWheelSpeed = 30.0f;
	float sensitivity = 20.0f;

	void UpdateProjectionMatrix();
	void UpdateViewMatrix();
	void SetOrientation();
	void SetViewport(unsigned width, unsigned height);
};


#endif
