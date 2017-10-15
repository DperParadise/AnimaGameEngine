#include "Grid.h"
#include "libraries/glew-2.0.0/include/GL/glew.h"

Grid::Grid() {}
Grid::~Grid() {}

void Grid::Draw()
{
	//---------------------------------------------- GRID DIRECT MODE ---------------------------------------
	glLineWidth(1.0f);
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	for (int z = -100; z <= 100; z += 1)
	{
		glVertex3i(-100, 0, z);
		glVertex3i(100, 0, z);
	}
	for (int x = -100; x <= 100; x += 1)
	{
		glVertex3i(x, 0, -100);
		glVertex3i(x, 0, 100);
	}

	glEnd();
}