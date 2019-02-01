#include "ComponentCamera.h"
#include "libraries/glm/gtc/matrix_transform.hpp"
#include "Application.h"
#include "ModuleWindow.h"


ComponentCamera::ComponentCamera(ComponentType type, GameObject *ownerGO) : 
	Component(type, ownerGO)
{
	aspectRatio = (float)App->window->window_width / (float)App->window->window_height;

	SetOrientation();
}

ComponentCamera:: ~ComponentCamera(){}

void ComponentCamera::Update(float dt)
{
	//camera behaviour logic
}

void ComponentCamera::OnResize(uint width, uint height)
{
	aspectRatio = (float)width / (float)height;
	UpdateProjectionMatrix();
}

const glm::mat4 &ComponentCamera::GetProjectionMatrix() const
{
	return projectionMatrix;
}

const glm::mat4 & ComponentCamera::GetViewMatrix() const
{
	return viewMatrix;
}

void ComponentCamera::UpdateProjectionMatrix()
{
	projectionMatrix = glm::perspective(glm::radians(verticalFOV), aspectRatio, nearPlane, farPlane);
}

void ComponentCamera::UpdateViewMatrix()
{
	viewMatrix = glm::lookAt(position, position + cameraFront, cameraUp);
}

void ComponentCamera::SetOrientation()
{
	glm::vec3 front;

	front.x = glm::cos(glm::radians(pitch))*cos(glm::radians(yaw));
	front.y = glm::sin(glm::radians(pitch));
	front.z = glm::cos(glm::radians(pitch))*sin(glm::radians(yaw));

	cameraFront = glm::normalize(front);
	cameraRight = glm::normalize(glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f)));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));

	UpdateViewMatrix();
}