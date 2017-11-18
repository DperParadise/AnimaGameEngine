#include "ComponentAmbientLight.h"

ComponentAmbientLight::ComponentAmbientLight(const std::string &name, bool act, GameObject *go) : Component(name, act, go)
{
	Enable();
}

ComponentAmbientLight::~ComponentAmbientLight(){}

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

void ComponentAmbientLight::SetDefaultAmbient() 
{
	memcpy(amb, default_amb, 4 * sizeof(float));
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
}

void ComponentAmbientLight::SetDarkAmbient()
{
	memcpy(amb, dark_amb, 4 * sizeof(float));
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
}

void ComponentAmbientLight::SetAmbient(float r, float g, float b, float a)
{
	amb[0] = r;
	amb[1] = g;
	amb[2] = b;
	amb[3] = a;

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
}