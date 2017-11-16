#ifndef __COMPONENT_CUBE_MESH_H__
#define __COMPONENT_CUBE_MESH_H__

#include "Component.h"
#include "libraries/glew-2.0.0/include/GL/glew.h"

class GameObject;
class ComponentCubeMesh : public Component
{

public:
	ComponentCubeMesh(component_type t, bool act, GameObject *go);
	virtual ~ComponentCubeMesh();

	virtual void Update(float dt);
	virtual void Enable();
	virtual void Disable();

private:
	static const unsigned NUM_VERT = 6 * 2 * 3;
	static const unsigned NUM_VERT_COORD = NUM_VERT * 3;
	
	GLfloat vertices[NUM_VERT_COORD] = {
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,

		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f,

		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f,

		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,

		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f,

		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f
	};	
};


#endif
