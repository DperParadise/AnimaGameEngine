#include "ComponentEditorCamera.h"
#include "Application.h"
#include "ModuleInput.h"
#include "libraries/SDL/include/SDL.h"

ComponentEditorCamera::ComponentEditorCamera(ComponentType type, GameObject *ownerGO) : ComponentCamera(type, ownerGO){}

void ComponentEditorCamera::Update(float dt) 
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
			pitch -= increment.y * sensitivity * dt;
			if (pitch > 89.0f)
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

	// Zoom in with mouse wheel -> warning: high values of FOV causes cam aberrations
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
}