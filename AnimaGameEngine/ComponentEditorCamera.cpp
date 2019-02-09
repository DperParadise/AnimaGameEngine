#include "ComponentEditorCamera.h"
#include "Application.h"
#include "ModuleInputGLFW.h"
//#include "libraries/SDL/include/SDL.h"
#include "Point.h"
#include <GLFW/glfw3.h>

ComponentEditorCamera::ComponentEditorCamera(ComponentType type, GameObject *ownerGO) : ComponentCamera(type, ownerGO){}

void ComponentEditorCamera::Update(float dt) 
{
	if (App->input->MouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
	{
		if (App->input->KeyPressed(GLFW_KEY_E))
		{
			if (App->input->KeyPressed(GLFW_KEY_LEFT_SHIFT))
				position.y += cameraSpeedFast * dt;
			else
				position.y += cameraSpeedSlow * dt;
		}

		if (App->input->KeyPressed(GLFW_KEY_Q))
		{
			if (App->input->KeyPressed(GLFW_KEY_LEFT_SHIFT))
				position.y -= cameraSpeedFast * dt;
			else
				position.y -= cameraSpeedSlow * dt;
		}

		if (App->input->KeyPressed(GLFW_KEY_W))
		{
			if (App->input->KeyPressed(GLFW_KEY_LEFT_SHIFT))
				position += cameraFront * cameraSpeedFast * dt;
			else
				position += cameraFront * cameraSpeedSlow * dt;
		}

		if (App->input->KeyPressed(GLFW_KEY_S))
		{
			if (App->input->KeyPressed(GLFW_KEY_LEFT_SHIFT))
				position -= cameraFront * cameraSpeedFast * dt;
			else
				position -= cameraFront * cameraSpeedSlow * dt;
		}

		if (App->input->KeyPressed(GLFW_KEY_A))
		{
			if (App->input->KeyPressed(GLFW_KEY_LEFT_SHIFT))
				position -= cameraRight * cameraSpeedFast * dt;
			else
				position -= cameraRight * cameraSpeedSlow * dt;
		}

		if (App->input->KeyPressed(GLFW_KEY_D))
		{
			if (App->input->KeyPressed(GLFW_KEY_LEFT_SHIFT))
				position += cameraRight * cameraSpeedFast * dt;
			else
				position += cameraRight * cameraSpeedSlow * dt;
		}

		//Use mouse motion to move the camera
		glm::vec2 increment = App->input->GetMouseOffset();

		yaw += increment.x * sensitivity * dt;
		pitch -= increment.y * sensitivity * dt;

		if (pitch > 89.0f)
			pitch = 89.0f;

		if (pitch < -89.0f)
			pitch = -89.0f;

		SetOrientation();

		MYLOG("position=(%f, %f, %f) pitch=%f yaw=%f", position.x, position.y, position.z, pitch, yaw)
	}

	//if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	//{
	//	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
	//	{
	//		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	//			position.y += cameraSpeedFast * dt;
	//		else
	//			position.y += cameraSpeedSlow * dt;
	//	}

	//	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)
	//	{
	//		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	//			position.y -= cameraSpeedFast * dt;
	//		else
	//			position.y -= cameraSpeedSlow * dt;
	//	}

	//	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	//	{
	//		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	//			position += cameraFront * cameraSpeedFast * dt;
	//		else
	//			position += cameraFront * cameraSpeedSlow * dt;

	//	}

	//	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	//	{
	//		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	//			position -= cameraFront * cameraSpeedFast * dt;
	//		else
	//			position -= cameraFront * cameraSpeedSlow * dt;
	//	}

	//	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	//	{
	//		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	//			position -= cameraRight * cameraSpeedFast * dt;
	//		else
	//			position -= cameraRight * cameraSpeedSlow * dt;
	//	}

	//	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	//	{
	//		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_REPEAT)
	//			position += cameraRight * cameraSpeedFast * dt;
	//		else
	//			position += cameraRight * cameraSpeedSlow * dt;
	//	}

	//	//Use mouse motion to move the camera
	//	iPoint increment = App->input->GetMouseMotion();

	//	yaw += increment.x * sensitivity * dt;
	//	pitch -= increment.y * sensitivity * dt;

	//	if (pitch > 89.0f)
	//		pitch = 89.0f;

	//	if (pitch < -89.0f)
	//		pitch = -89.0f;

	//	SetOrientation();

	//}

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