#include "ComponentLight.h"

int ComponentLight::lights_counter = -1;

ComponentLight::ComponentLight(ComponentType type, const std::string &name, bool act, GameObject *go) : Component(type, name, act, go)
{
	lights_counter++;
	light_index = lights_counter;

	glLightfv(SelectLight(), GL_AMBIENT, ambient);
	glLightfv(SelectLight(), GL_DIFFUSE, diffuse);
	glLightfv(SelectLight(), GL_SPECULAR, specular);

	if(name == "Directional Light")
		glLightfv(SelectLight(), GL_POSITION, direction);
	else
		glLightfv(SelectLight(), GL_POSITION, position);
	
	Enable();
}

ComponentLight::~ComponentLight() 
{
	lights_counter--;
	Disable();
}

void ComponentLight::Update(float dt){}

void ComponentLight::Enable()
{
	glEnable(GL_LIGHTING);
	glEnable(SelectLight());
	active = true;
}

void ComponentLight::Disable()
{
	glDisable(SelectLight());
	active = false;
}

GLenum ComponentLight::SelectLight()
{	
	GLenum light = lights[light_index];
	return lights[light_index];	
}

void ComponentLight::SetPosition(float x, float y, float z)
{
	position[0] = x;
	position[1] = y;
	position[2] = z;
	position[3] = 1.0f;

	glLightfv(SelectLight(), GL_POSITION, position);
}

void ComponentLight::SetDirection(float x, float y, float z)
{
	position[0] = x;
	position[1] = y;
	position[2] = z;
	position[3] = 0.0f;

	glLightfv(SelectLight(), GL_POSITION, position);
}

void ComponentLight::SetSpotDirection(float x, float y, float z)
{
	spot_direction[0] = x;
	spot_direction[1] = y;
	spot_direction[2] = z;

	glLightfv(SelectLight(), GL_SPOT_DIRECTION, spot_direction);
}

void ComponentLight::SetCutoff(float coff)
{
	if (coff < 0.0f)
		coff = 0.0f;

	if (coff > 90.0f)
		coff = 180.0f;

	glLightfv(SelectLight(), GL_SPOT_CUTOFF, &coff);
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