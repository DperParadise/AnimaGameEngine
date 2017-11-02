#include "ComponentLight.h"

int ComponentLight::lights_counter = -1;

ComponentLight::ComponentLight(light_type lt, component_type t, bool act, GameObject *go) : Component(t, act, go)
{
	lights_counter++;
	light_index = lights_counter;

	glLightfv(SelectLight(), GL_AMBIENT, ambient);
	glLightfv(SelectLight(), GL_DIFFUSE, diffuse);
	glLightfv(SelectLight(), GL_SPECULAR, specular);
	glLightfv(SelectLight(), GL_POSITION, position);
	
	Enable();
}

ComponentLight::~ComponentLight() 
{
	lights_counter--;
	Disable();
}

void ComponentLight::Update(){}

void ComponentLight::Enable()
{
	glEnable(SelectLight());
	glEnable(GL_LIGHTING);
	active = true;
}

void ComponentLight::Disable()
{
	glDisable(SelectLight());
	active = false;
}

GLenum ComponentLight::SelectLight()
{	
	return lights[light_index];	
}

void ComponentLight::SetPosition(float x, float y, float z, float w)
{
	position[0] = x;
	position[1] = y;
	position[2] = z;
	position[3] = w;

	glLightfv(SelectLight(), GL_POSITION, position);
}

void ComponentLight::SetAmbient(float r, float g, float b, float a)
{
	ambient[0] = r;
	ambient[1] = g;
	ambient[2] = b;
	ambient[3] = a;

	glLightfv(SelectLight(), GL_AMBIENT, ambient);
}

void ComponentLight::SetDiffuse(float r, float g, float b, float a)
{
	diffuse[0] = r;
	diffuse[1] = g;
	diffuse[2] = b;
	diffuse[3] = a;

	glLightfv(SelectLight(), GL_DIFFUSE, diffuse);
}

void ComponentLight::SetSpecular(float r, float g, float b, float a)
{
	specular[0] = r;
	specular[1] = g;
	specular[2] = b;
	specular[3] = a;

	glLightfv(SelectLight(), GL_SPECULAR, specular);
}