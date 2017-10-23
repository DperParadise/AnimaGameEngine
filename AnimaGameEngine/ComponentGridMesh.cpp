#include "ComponentGridMesh.h"
#include "libraries/assimp/include/assimp/vector3.h"
#include <vector>
#include "GameObject.h"
#include "ComponentTransform.h"
#include "libraries/glew-2.0.0/include/GL/glew.h"


ComponentGridMesh::ComponentGridMesh(component_type t, bool act, GameObject *go) : Component(t, act, go) {}

ComponentGridMesh::~ComponentGridMesh() {}

void ComponentGridMesh::Update()
{
	//draw the mesh
	aiVector3D position;

	for (std::vector<Component*>::iterator it = owner_go->components.begin(); it != owner_go->components.end(); it++)
	{
		if ((*it)->type == component_type::TRANSFORM)
			position = ((ComponentTransform*)(*it))->position;
	}

	glLineWidth(0.5f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
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
	glPopMatrix();
}

void ComponentGridMesh::Enable()
{
	active = true;
}

void ComponentGridMesh::Disable()
{
	active = false;
}