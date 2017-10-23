#ifndef __COMPONENT_SPHERE_MESH__
#define __COMPONENT_SPHERE_MESH__

#include "Component.h"
#include "libraries/glew-2.0.0/include/GL/glew.h"

class GLUquadric;
class ComponentSphereMesh : public Component
{
public:
	ComponentSphereMesh(component_type t, bool act, GameObject *go);
	virtual ~ComponentSphereMesh();

	virtual void Update();
	virtual void Enable();
	virtual void Disable();

	void SetRadius(GLuint r);
	void SetSlices(GLuint sl);
	void SetStacks(GLdouble st);

private:
	GLUquadric *quadric = nullptr;
	GLuint radius = 1;
	GLuint slices = 10;
	GLuint stacks = 10;
};

#endif
