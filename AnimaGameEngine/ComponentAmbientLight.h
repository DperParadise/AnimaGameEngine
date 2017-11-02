#ifndef __COMPONENT_AMBIENT_LIGHT_H__
#define __COMPONENT_AMBIENT_LIGHT_H__

#include "Component.h"
#include "libraries/glew-2.0.0/include/GL/glew.h"

class ComponentAmbientLight : public Component
{
public:
	ComponentAmbientLight(component_type t, bool act, GameObject *go);
	~ComponentAmbientLight();

	void Update();
	void Enable();
	void Disable();

	void SetDefaultAmbientL();
	void SetDarkAmbientL();
	void SetAmbientL(float r, float g, float b, float a);

private:
	GLfloat default_amb[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat amb[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat dark_amb[4] = { 0.2f, 0.2f, 0.2f, 0.0f };

};

#endif
