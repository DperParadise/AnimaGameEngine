#include "ComponentAmbientLight.h"
#include "libraries/glew-2.0.0/include/GL/glew.h"
#include <cstring>

ComponentAmbientLight::ComponentAmbientLight(component_type t, bool act, GameObject *go) : Component(t, act, go)
{
	Enable();
}

ComponentAmbientLight::~ComponentAmbientLight() 
{
	Disable();
}

void ComponentAmbientLight::Update(float dt){}

void ComponentAmbientLight::Enable()
{
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
	glEnable(GL_LIGHTING);
	active = true;
}

void ComponentAmbientLight::Disable()
{
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, dark_amb);
	active = false;
}

void ComponentAmbientLight::SetDefaultAmbientL() 
{
	memcpy(amb, default_amb, 4 * sizeof(float));
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
}

void ComponentAmbientLight::SetDarkAmbientL()
{
	memcpy(amb, dark_amb, 4 * sizeof(float));
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
}

void ComponentAmbientLight::SetAmbientL(float r, float g, float b, float a)
{
	amb[0] = r;
	amb[1] = g;
	amb[2] = b;
	amb[3] = a;

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
}