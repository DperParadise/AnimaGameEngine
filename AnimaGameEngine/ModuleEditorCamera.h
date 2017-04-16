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

	void SetVerticalFOV(float vertical_fov);
	void SetAspectRatio(float aspect_ratio);
	void SetPlaneDistances(float near, float far);
	void SetPosition(float3 pos);
	void SetOrientation(float3 front, float3 up);
	void LookAt(float x, float y, float z);
	void ComputeProjectionMatrix();
	void ComputeViewMatrix();

	float GetHorizontalFOV(float vertical_fov);
	

	Frustum camera;
	float projectionMatrix[16];
	float viewMatrix[16];

private:
	FrustumProjectiveSpace projectiveSpace = FrustumProjectiveSpace::FrustumSpaceGL;
	FrustumHandedness handedness = FrustumHandedness::FrustumRightHanded;
	vec position = vec(0.0f, 0.0f, 5.0f);
	vec front_vect = vec(0.0f, 0.0f, -1.0f);
	vec up_vect = vec(0.0f, 1.0f, 0.0f);
	float near_plane = 1.0f;
	float far_plane = 100.0f;
	float vertical_fov = 60.0;
	float horizontal_fov;
	int width = 1024;
	int height = 768;
	float aspect_ratio = 1.3333;

};


#endif
