#include "CameraGO.h"
#include "Component.h"

CameraGO::CameraGO(const std::string &name) : GameObject(name)
{
	AddCameraComponent();
}