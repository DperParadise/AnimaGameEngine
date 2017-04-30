#ifndef __MODULECAMEDITOR_H__
#define __MODULECAMEDITOR_H__

#include "Globals.h"
#include "Module.h"
#include "libraries/MathGeoLib/MathGeoLib.h"


class Config;
class ModuleEditorCamera : public Module
{
public:
	ModuleEditorCamera();
	virtual ~ModuleEditorCamera();

	bool Init(Config *config);
	update_status Update(float dt);
	bool CleanUp();

	void SetVerticalFOVAndAspectRatio(float vertical_fov, int width, int height);
	float GetVerticalFOV();
	float GetAspectRatio();
	void SetPlaneDistances(float near, float far);
	void SetPosition(const float3 &pos);
	void SetOrientation(const float3 &front, const float3 &up);
	void LookAt(const float3 &point);
	void ComputeProjectionMatrix();
	void ComputeViewMatrix();

	void OnResize(int window_width, int window_height);

	Frustum frustum;
	float projectionMatrix[16];
	float viewMatrix[16];

private:
	FrustumProjectiveSpace projectiveSpace = FrustumProjectiveSpace::FrustumSpaceGL;
	FrustumHandedness handedness = FrustumHandedness::FrustumRightHanded;
	float3 position = float3(0.0f, 5.0f, 10.0f);
	float3 front_vect = float3(0.0f, 0.0f, -1.0f);
	float3 up_vect = float3(0.0f, 1.0f, 0.0f);
	float near_plane = 1.0f;
	float far_plane = 100.0f;
	float vertical_fov = 60.0f;
	float horizontal_fov;
	int width = 1024;
	int height = 768;
	float aspect_ratio = 1.3333f;
	float camera_speed = 30.0f;

};


#endif
