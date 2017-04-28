#include "ModuleEditorCamera.h"


ModuleEditorCamera::ModuleEditorCamera() {}

ModuleEditorCamera::~ModuleEditorCamera() {}

bool ModuleEditorCamera::Init(Config *config)
{
	//añadir lectura de propiedades desde el archivo de configuración
	
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
void ModuleEditorCamera::LookAt(const float3 &position)
{	
	float3 dir = position - frustum.Pos();
	float3x3 m = float3x3::LookAt(frustum.Front(), dir.Normalized(), frustum.Up(), float3::unitY);
	float3 front = m.MulDir(frustum.Front()).Normalized();
	float3 up = m.MulDir(frustum.Up()).Normalized();
	SetOrientation(front, up);
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