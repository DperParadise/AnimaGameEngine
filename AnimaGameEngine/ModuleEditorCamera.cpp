#include "ModuleEditorCamera.h"


ModuleEditorCamera::ModuleEditorCamera() {}

ModuleEditorCamera::~ModuleEditorCamera() {}

bool ModuleEditorCamera::Init(Config *config)
{
	//a�adir lectura de propiedades desde el archivo de configuraci�n
	
	frustum.SetKind(projectiveSpace, handedness);
	frustum.SetPos(position);
	frustum.SetFront(front_vect);
	frustum.SetUp(up_vect);
	frustum.SetViewPlaneDistances(near_plane, far_plane);
	frustum.SetVerticalFovAndAspectRatio(DegToRad(vertical_fov), aspect_ratio);

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

void ModuleEditorCamera::SetVerticalFOVAndAspectRatio(float vertical_fov, int width, int height)
{
	aspect_ratio = (float)width / height;
	frustum.SetVerticalFovAndAspectRatio(DegToRad(vertical_fov), aspect_ratio);
	ComputeProjectionMatrix();
}

float ModuleEditorCamera::GetVerticalFOV()
{
	return vertical_fov;
}

float ModuleEditorCamera::GetAspectRatio()
{
	return aspect_ratio;
}

void ModuleEditorCamera::SetPlaneDistances(float near, float far)
{
	near_plane = near;
	far_plane = far;
	frustum.SetViewPlaneDistances(near_plane, far_plane);
	ComputeProjectionMatrix();
}
void ModuleEditorCamera::SetPosition(const float3 &pos)
{
	position = pos;
	frustum.SetPos(position);
	ComputeProjectionMatrix();
	ComputeViewMatrix();
}
void ModuleEditorCamera::SetOrientation(const float3 &front, const float3 &up)
{
	front_vect = front;
	up_vect = up;
	frustum.SetFront(front_vect);
	frustum.SetUp(up_vect);
	ComputeProjectionMatrix();
	ComputeViewMatrix();
}
void ModuleEditorCamera::LookAt(const float3 &point)
{
	float3 new_right;
	float3 new_up;
	float3 new_front = (point - frustum.Pos()).Normalized();

	float3 left_right = frustum.Front().Cross(new_front);

	if ((int)left_right.Length() != 0) // check whether front and new front look at the same point
	{
		float3 check_vector = left_right.Cross(frustum.Front());
		float test_product = frustum.Up().Dot(check_vector);
		if (test_product > 0)
			new_right = left_right;
		else if (test_product < 0)
			new_right = -1 * left_right;

		new_up = new_right.Cross(new_front);

		SetOrientation(new_front, new_up);
	
	}
	
}

void ModuleEditorCamera::ComputeProjectionMatrix()
{
	float4x4 matrix = frustum.ComputeProjectionMatrix();
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
	float4x4 matrix = frustum.ComputeViewMatrix();
	matrix.Transpose();

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			viewMatrix[i * 4 + j] = matrix[i][j];
		}
	}
}

void ModuleEditorCamera::OnResize(int window_width, int window_height)
{	
	SetVerticalFOVAndAspectRatio(vertical_fov, window_width, window_height);
	ComputeProjectionMatrix();
}