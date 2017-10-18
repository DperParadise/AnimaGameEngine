#include "SpherePrimitive.h"

SpherePrimitive::SpherePrimitive()
{
	quadric = gluNewQuadric();
}

SpherePrimitive::~SpherePrimitive()
{
	gluDeleteQuadric(quadric);
}

void SpherePrimitive::Draw(GLdouble radius, GLint slices, GLint stacks, GLfloat x, GLfloat y, GLfloat z)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(x, y, z);
	gluQuadricDrawStyle(quadric, GLU_LINE);
	gluSphere(quadric, radius, slices, stacks);
	glPopMatrix();

}

void SpherePrimitive::Clear()
{
	gluDeleteQuadric(quadric);
}
