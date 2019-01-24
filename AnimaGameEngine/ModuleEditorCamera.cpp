#include "ModuleEditorCamera.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "libraries/glm/gtc/matrix_transform.hpp"
#include "libraries/SDL/include/SDL.h"

ModuleEditorCamera::ModuleEditorCamera() {}

ModuleEditorCamera::~ModuleEditorCamera() {}

bool ModuleEditorCamera::Init(Config *config) 
{
	aspectRatio = (float)App->window->window_width / (float)App->window->window_height;
	
	SetOrientation();

	return true;
}

update_status ModuleEditorCamera::Update(float dt)
{
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
		{
			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
				position.y += cameraSpeedFast * dt;
			else
				position.y += cameraSpeedSlow * dt;
		}

		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)
		{
			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
				position.y -= cameraSpeedFast * dt;
			else
				position.y -= cameraSpeedSlow * dt;
		}
	
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
				position += cameraFront * cameraSpeedFast * dt;
			else
				position += cameraFront * cameraSpeedSlow * dt;

		}

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
				position -= cameraFront * cameraSpeedFast * dt;
			else
				position -= cameraFront * cameraSpeedSlow * dt;
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
				position -= cameraRight * cameraSpeedFast * dt;
			else
				position -= cameraRight * cameraSpeedSlow * dt;
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_REPEAT)
				position += cameraRight * cameraSpeedFast * dt;
			else
				position += cameraRight * cameraSpeedSlow * dt;
		}

		//Use mouse motion to move the camera
		iPoint increment = App->input->GetMouseMotion();
		if (increment.y != 0)
		{
			pitch -= increment.y * sensitivity * dt ;
			if (pitch > 89.0f )
				pitch = 89.0f;

			if (pitch < -89.0f)
				pitch = -89.0f;
		}
		if (increment.x != 0)
		{
			yaw += increment.x * sensitivity * dt;
		}

		SetOrientation();
	}

	// Zoom in with mouse wheel
	/*
	if (App->input->IsScrollingUp())
	{	
		verticalFOV -=  cameraWheelSpeed * dt;
		UpdateProjectionMatrix();
		MYLOG("vertical FOV = %f", verticalFOV)
	}
	*/
	// Zoom out with mouse wheel
	/*
	if (App->input->IsScrollingDown())
	{
		verticalFOV += cameraWheelSpeed * dt;
		UpdateProjectionMatrix();
		MYLOG("vertical FOV = %f", verticalFOV)
	}
	*/
	return UPDATE_CONTINUE;
}

bool ModuleEditorCamera::CleanUp()
{
	return true;
}

void ModuleEditorCamera::OnResize()
{
	aspectRatio = (float)App->window->window_width / (float)App->window->window_height;
	UpdateProjectionMatrix();
}

const glm::mat4 & ModuleEditorCamera::GetProjectionMatrix() const
{
	return projectionMatrix;
}

const glm::mat4 & ModuleEditorCamera::GetViewMatrix() const
{
	return viewMatrix;
}

void ModuleEditorCamera::UpdateProjectionMatrix()
{
	projectionMatrix = glm::perspective(glm::radians(verticalFOV), aspectRatio, nearPlane, farPlane);
}

void ModuleEditorCamera::UpdateViewMatrix()
{
	viewMatrix = glm::lookAt(position, position + cameraFront, cameraUp);
}

void ModuleEditorCamera::SetOrientation()
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



