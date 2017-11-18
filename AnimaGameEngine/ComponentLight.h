#ifndef __COMPONENT_LIGHT_H__
#define __COMPONENT_LIGHT_H__

#include "Component.h"
#include "libraries/glew-2.0.0/include/GL/glew.h"
#include <string>

class ComponentLight : public Component
{
public:

	ComponentLight(const std::string &name, bool act, GameObject *go);
	~ComponentLight();

	void Update(float dt);
	void Enable();
	void Disable();
	void SetPosition(float x, float y, float z);//arreglarlo : la transform del gameobject afecta.
	void SetDirection(float x, float y, float z);
	void SetSpotDirection(float x, float y, float z);//arreglarlo : la transform del gameobject afecta.
	void SetCutoff(float coff);
	void SetAmbient(float r, float g, float b, float a);
	void SetDiffuse(float r, float g, float b, float a);
	void SetSpecular(float r, float g, float b, float a);


	static int lights_counter;
	unsigned int light_index = 0;

private:

	float ambient[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float position[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float direction[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	float spot_direction[3] = { 0.0f, 0.0f, -1.0f };
	float cutoff = 45.0f;
	GLenum SelectLight();
	GLenum lights[8] = { GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, 
						GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7 };
};





#endif
