#ifndef __SPHERE_H__
#define __SPHERE_H__

#include <glew_2.0.0/GL/glew.h>

class GLUquadric;
class SpherePrimitive
{
public:
	SpherePrimitive();
	~SpherePrimitive();

	void Draw(GLdouble radius, GLint slices, GLint stacks, GLfloat x, GLfloat y, GLfloat z);
	void Clear();

private:
	GLUquadric *quadric = nullptr;
};



#endif
