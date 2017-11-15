#include "Gizmo.h"
#include "libraries/glew-2.0.0/include/GL/glew.h"
#include "Globals.h"
#include "GameObject.h"

Gizmo::Gizmo() 
{
	vertices = new float[num_vertices_coord] {		
		//axis X
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f,0.0f,
		//arrow X
		0.75f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.75f,-0.25f,0.0f, 1.0f, 0.0f, 0.0f,
		//label X
		1.0f, 0.25f, 0.0f, 1.5f, -0.25f, 0.0f,
		1.0f, -0.25f, 0.0f, 1.5f, 0.25f, 0.0f,
		//axis Y
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		//arrow Y
		0.0f, 1.0f, 0.0f, 0.25f, 0.75, 0.0f,
		0.0f, 1.0f, 0.0f, -0.25f, 0.75f, 0.0f,
		//label Y
		0.0f, 1.30f, 0.0f, 0.25f, 1.55f, 0.0f,
		0.0f, 1.30f, 0.0f, -0.25f, 1.55f, 0.0f,
		0.0f, 1.30f, 0.0f, 0.0f, 1.05f, 0.0f,
		//axis Z
		0.f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		//arrow Z
		0.0f, 0.0f, 1.0f, 0.25f, 0.0f, 0.75f,
		0.0f, 0.0f, 1.0f, -0.25f, 0.0f, 0.75f,
		//label Z
		-0.25f, 0.25f, 1.25f, 0.25f, 0.25f, 1.25f,
		0.25f, 0.25f, 1.25f, -0.25f, -0.25f, 1.25f,
		-0.25f, -0.25f, 1.25f, 0.25f, -0.25f, 1.25f
	};
	colors = new float[num_vertices_coord] {
		//axis X
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			//arrow X
			1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			//label X
			1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			//axis Y
			0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			//arrow Y
			0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			//label Y
			0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			//axis Z
			0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			//arrow Z
			0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			//label Z
			0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

}
Gizmo::~Gizmo() 
{
	RELEASE_ARRAY(vertices);
	RELEASE_ARRAY(colors);
}

void Gizmo::Draw(const GameObject *go) const
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	
	glTranslatef(go->transform.world_position.x, go->transform.world_position.y, go->transform.world_position.z);

	aiMatrix3x3 rot = go->transform.world_rotation.GetMatrix(); //matrix in row-major order. Transpose before feeding openGL 
	GLfloat rot4x4[16] = { 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			rot4x4[i * 4 + j] = rot[i][j];
		}
	}

	//transpose rot4x4
	GLfloat t_rot4x4[16];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			t_rot4x4[i * 4 + j] = rot4x4[j * 4 + i];
		}
	}

	glMultMatrixf(t_rot4x4);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, colors);
	glLineWidth(2.0f);
	glEnable(GL_COLOR_MATERIAL);
	glDrawArrays(GL_LINES, 0, num_vertices);
	glDisable(GL_COLOR_MATERIAL);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);


	////---------------------------------------------- GIZMO (0,0,0) -----------------
	//glLineWidth(2.0f);
	//glBegin(GL_LINES);
	//glColor3f(3.0f, 0.0f, 0.0f);
	////axis X
	//glVertex3i(0, 0, 0);
	//glVertex3i(1, 0, 0);
	////arrow X
	//glVertex3f(0.75f, 0.25f, 0.0f);
	//glVertex3f(1.0f, 0.0f, 0.0f);
	//glVertex3f(0.75f, -0.25f, 0.0f);
	//glVertex3f(1.0f, 0.0f, 0.0f);
	////label X
	//glVertex3f(1.0f, 0.25f, 0.0f);
	//glVertex3f(1.5f, -0.25f, 0.0f);
	//glVertex3f(1.0f, -0.25f, 0.0f);
	//glVertex3f(1.5f, 0.25f, 0.0f);

	//glColor3f(0.0f, 3.0f, 0.0f);
	////axis Y
	//glVertex3i(0, 0, 0);
	//glVertex3i(0, 1, 0);
	////arrow Y
	//glVertex3f(0.0f, 1.0f, 0.0f);
	//glVertex3f(0.25f, 0.75, 0.0f);
	//glVertex3f(0.0f, 1.0f, 0.0f);
	//glVertex3f(-0.25f, 0.75f, 0.0f);
	////label Y
	//glVertex3f(0.0f, 1.30f, 0.0f);
	//glVertex3f(0.25f, 1.55f, 0.0f);
	//glVertex3f(0.0f, 1.30f, 0.0f);
	//glVertex3f(-0.25f, 1.55f, 0.0f);
	//glVertex3f(0.0f, 1.30f, 0.0f);
	//glVertex3f(0.0f, 1.05f, 0.0f);

	//glColor3f(0.0f, 0.0f, 3.0);
	////axis Z
	//glVertex3i(0, 0, 0);
	//glVertex3i(0, 0, 1);
	////arrow Z
	//glVertex3f(0.0f, 0.0f, 1.0f);
	//glVertex3f(0.25f, 0.0f, 0.75f);
	//glVertex3f(0.0f, 0.0f, 1.0f);
	//glVertex3f(-0.25f, 0.0f, 0.75f);
	////label Z
	//glVertex3f(-0.25f, 0.25f, 1.25f);
	//glVertex3f(0.25f, 0.25f, 1.25f);
	//glVertex3f(0.25f, 0.25f, 1.25f);
	//glVertex3f(-0.25f, -0.25f, 1.25f);
	//glVertex3f(-0.25f, -0.25f, 1.25f);
	//glVertex3f(0.25f, -0.25f, 1.25f);

	//glEnd();
}