#include "ModuleEditorCamera.h"


ModuleEditorCamera::ModuleEditorCamera() {}

ModuleEditorCamera::~ModuleEditorCamera() {}

bool ModuleEditorCamera::Init(Config *config)
{
	
	camera.SetKind(projectiveSpace, handedness);
	camera.SetPos(position);
	camera.SetFront(front_vect);
	camera.SetUp(up_vect);
	camera.SetViewPlaneDistances(near_plane, far_plane);
	camera.SetVerticalFovAndAspectRatio(DegToRad(vertical_fov), aspect_ratio);

	memset(projectionMatrix, 0, 16 * sizeof(float));
	memset(viewMatrix, 0, 16 * sizeof(float));

	//---------------------------------------- SET DEFAULT PROJECTION AND VIEW MATRICES ---------------------------------
	ComputeProjectionMatrix();
	ComputeViewMatrix();

	return true;
}

update_status ModuleEditorCamera::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleEditorCamera::CleanUp()
{
	return true;
}

void ModuleEditorCamera::SetVerticalFOV(float vertical_fov)
{
	camera.SetHorizontalFovAndAspectRatio(vertical_fov, aspect_ratio);
}

void ModuleEditorCamera::SetAspectRatio(float aspect_ratio)
{
	aspect_ratio = width / height;
}
void ModuleEditorCamera::SetPlaneDistances(float near, float far)
{
	near_plane = near;
	far_plane = far;
}
void ModuleEditorCamera::SetPosition(float3 pos)
{
	position = pos;
}
void ModuleEditorCamera::SetOrientation(float3 front, float3 up)
{
	front_vect = front;
	up_vect = up;
}
void ModuleEditorCamera::LookAt(float x, float y, float z)
{
	//ver lookAt de float4x4 de MathGeoLib
}

void ModuleEditorCamera::ComputeProjectionMatrix()
{
	float4x4 matrix = camera.ComputeProjectionMatrix();
	matrix.Transpose();
	
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			projectionMatrix[i * 4 + j] = matrix[i][j];
		}
	}
}

void ModuleEditorCamera::ComputeViewMatrix()
{
	float4x4 matrix = camera.ComputeViewMatrix();
	matrix.Transpose();

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			viewMatrix[i * 4 + j] = matrix[i][j];
		}
	}
}

float ModuleEditorCamera::GetHorizontalFOV(float vertical_fov)
{
	horizontal_fov = 2 * atan(tan(vertical_fov / 2) * aspect_ratio);

	return horizontal_fov;
}
