#include "Gizmo.h"
#include "libraries/glew-2.0.0/include/GL/glew.h"

Gizmo::Gizmo() {}
Gizmo::~Gizmo() {}

void Gizmo::Draw()
{
	//---------------------------------------------- GIZMO (0,0,0) -----------------
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(3.0f, 0.0f, 0.0f);
	//axis X
	glVertex3i(0, 0, 0);
	glVertex3i(1, 0, 0);
	//arrow X
	glVertex3f(0.75f, 0.25f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.75f, -0.25f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	//label X
	glVertex3f(1.0f, 0.25f, 0.0f);
	glVertex3f(1.5f, -0.25f, 0.0f);
	glVertex3f(1.0f, -0.25f, 0.0f);
	glVertex3f(1.5f, 0.25f, 0.0f);

	glColor3f(0.0f, 3.0f, 0.0f);
	//axis Y
	glVertex3i(0, 0, 0);
	glVertex3i(0, 1, 0);
	//arrow Y
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.25f, 0.75, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.25f, 0.75f, 0.0f);
	//label Y
	glVertex3f(0.0f, 1.30f, 0.0f);
	glVertex3f(0.25f, 1.55f, 0.0f);
	glVertex3f(0.0f, 1.30f, 0.0f);
	glVertex3f(-0.25f, 1.55f, 0.0f);
	glVertex3f(0.0f, 1.30f, 0.0f);
	glVertex3f(0.0f, 1.05f, 0.0f);

	glColor3f(0.0f, 0.0f, 3.0);
	//axis Z
	glVertex3i(0, 0, 0);
	glVertex3i(0, 0, 1);
	//arrow Z
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.25f, 0.0f, 0.75f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.25f, 0.0f, 0.75f);
	//label Z
	glVertex3f(-0.25f, 0.25f, 1.25f);
	glVertex3f(0.25f, 0.25f, 1.25f);
	glVertex3f(0.25f, 0.25f, 1.25f);
	glVertex3f(-0.25f, -0.25f, 1.25f);
	glVertex3f(-0.25f, -0.25f, 1.25f);
	glVertex3f(0.25f, -0.25f, 1.25f);

	glEnd();
}