#ifndef __COMPONENT_LIGHT_H__
#define __COMPONENT_LIGHT_H__

#include "Component.h"
#include "libraries/glew-2.0.0/include/GL/glew.h"

class ComponentLight : public Component
{
public:

	enum class light_type
	{
		DIRECTIONAL,
		POINT
	};
	
	light_type light_t = light_type::POINT;

	ComponentLight(light_type lt, component_type t, bool act, GameObject *go);
	~ComponentLight();

	void Update();
	void Enable();
	void Disable();
	void SetPosition(float x, float y, float z, float w); //Sets the position for point lights or direction for directional lights
	void SetAmbient(float r, float g, float b, float a);
	void SetDiffuse(float r, float g, float b, float a);
	void SetSpecular(float r, float g, float b, float a);

	static int lights_counter;
	unsigned int light_index = 0;

private:

	float ambient[4] = { 1.0f, 1.0f, 1.0f, 0.1f };
	float diffuse[4] = { 1.0f, 1.0f, 1.0f, 0.1f };
	float specular[4] = { 1.0f, 1.0f, 1.0f, 0.1f };
	float position[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLenum SelectLight();
	GLenum lights[8] = { GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, 
						GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7 };
};





#endif
