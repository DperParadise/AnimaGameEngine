#include "ModuleEditorCamera.h"
#include "Application.h"
#include "ModuleInput.h"

ModuleEditorCamera::ModuleEditorCamera() {}

ModuleEditorCamera::~ModuleEditorCamera() {}

bool ModuleEditorCamera::Init(Config *config)
{
	//TODO : añadir lectura de propiedades desde el archivo de configuración
	
	frustum.SetKind(projectiveSpace, handedness);
	frustum.SetPos(position);
	frustum.SetFront(front_vect);
	frustum.SetUp(up_vect);
	frustum.SetViewPlaneDistances(near_plane, far_plane);
	frustum.SetVerticalFovAndAspectRatio(DegToRad(vertical_fov), aspect_ratio);

	memset(projectionMatrix, 0, 16 * sizeof(float));
	memset(viewMatrix, 0, 16 * sizeof(float));

	//---------------------------------------- SET DEFAULT PROJECTION AND VIEW MATRICES ---------------------------------
	lookat_point = float3::zero;
	LookAt(lookat_point);

	//ComputeProjectionMatrix();
	//ComputeViewMatrix();


	return true;
}

update_status ModuleEditorCamera::Update(float dt)
{
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
		{
			float3 tmp = frustum.Pos();

			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_REPEAT)
				tmp.y += camera_speed_fast * dt;
			else
				tmp.y += camera_speed * dt;

			SetPosition(tmp);
		}

		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)
		{
			float3 tmp = frustum.Pos();

			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_REPEAT)
				tmp.y -= camera_speed_fast * dt;
			else
				tmp.y -= camera_speed * dt;

			SetPosition(tmp);
		}
	
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			float3 tmp = frustum.Pos();

			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_REPEAT)
				tmp += frustum.Front() * camera_speed_fast * dt;
			else
				tmp += frustum.Front() * camera_speed * dt;

			SetPosition(tmp);
		}

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			float3 tmp = frustum.Pos();

			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_REPEAT)
				tmp -= frustum.Front() * camera_speed_fast * dt;
			else
				tmp -= frustum.Front() * camera_speed * dt;

			SetPosition(tmp);
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			float3 tmp = frustum.Pos();

			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_REPEAT)
				tmp -= frustum.WorldRight() * camera_speed_fast * dt;
			else
				tmp -= frustum.WorldRight() * camera_speed * dt;

			SetPosition(tmp);
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			float3 tmp = frustum.Pos();

			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_REPEAT)
				tmp += frustum.WorldRight() * camera_speed_fast * dt;
			else
				tmp += frustum.WorldRight() * camera_speed * dt;
			SetPosition(tmp);
		}

		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{		
			float3 new_front = frustum.Front() + frustum.Up() * 0.5 * dt;
			float3x3 m = float3x3::LookAt(frustum.Front(), new_front.Normalized(), frustum.Up(), float3::unitY);
			float3 front = m.MulDir(frustum.Front()).Normalized();
			float3 up = m.MulDir(frustum.Up()).Normalized();
			SetOrientation(front, up);
		}

		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			float3 up_increment = frustum.Up();
			float3 new_front = frustum.Front() - frustum.Up() * 0.5 * dt;
			float3x3 m = float3x3::LookAt(frustum.Front(), new_front.Normalized(), frustum.Up(), float3::unitY);
			float3 front = m.MulDir(frustum.Front()).Normalized();
			float3 up = m.MulDir(frustum.Up()).Normalized();
			SetOrientation(front, up);
		}

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{	
			float3 new_front = frustum.Front() - frustum.WorldRight() * 0.5 * dt;
			float3x3 m = float3x3::LookAt(frustum.Front(), new_front.Normalized(), frustum.Up(), float3::unitY);
			float3 front = m.MulDir(frustum.Front()).Normalized();
			float3 up = m.MulDir(frustum.Up()).Normalized();
			SetOrientation(front, up);
		}

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{		
			float3 new_front = frustum.Front() + frustum.WorldRight() * 0.5 * dt;
			float3x3 m = float3x3::LookAt(frustum.Front(), new_front.Normalized(), frustum.Up(), float3::unitY);
			float3 front = m.MulDir(frustum.Front()).Normalized();
			float3 up = m.MulDir(frustum.Up()).Normalized();
			SetOrientation(front, up);
		}

		//Use mouse motion to move the camera
		iPoint increment = App->input->GetMouseMotion();
		if (increment.y != 0)
		{	
			float3 new_front = frustum.Front() - increment.y * sensitivity * frustum.Up(); //debería * dt pero tiene variaciones de dt muy feas. Revisarlo
			float3x3 m = float3x3::LookAt(frustum.Front(), new_front.Normalized(), frustum.Up(), float3::unitY);
			float3 front = m.MulDir(frustum.Front()).Normalized();
			float3 up = m.MulDir(frustum.Up()).Normalized();
			SetOrientation(front, up);
			
		}

		if (increment.x != 0)
		{
			float3 new_front = frustum.Front() + increment.x * sensitivity * frustum.WorldRight(); //debería * dt pero tiene variaciones de dt muy feas. Revisarlo
			float3x3 m = float3x3::LookAt(frustum.Front(), new_front.Normalized(), frustum.Up(), float3::unitY);
			float3 front = m.MulDir(frustum.Front()).Normalized();
			float3 up = m.MulDir(frustum.Up()).Normalized();
			SetOrientation(front, up);
		}
	}

	// Move forward with mouse wheel
	if (App->input->IsScrollingUp())
	{
		float3 tmp = frustum.Pos();
		tmp += frustum.Front() * camera_wheel_speed * dt;
		SetPosition(tmp);
	}

	// Move backward with mouse wheel
	if (App->input->IsScrollingDown())
	{
		float3 tmp = frustum.Pos();
		tmp -= frustum.Front() * camera_wheel_speed * dt;
		SetPosition(tmp);
	}

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
	float3 dir = point - frustum.Pos();
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