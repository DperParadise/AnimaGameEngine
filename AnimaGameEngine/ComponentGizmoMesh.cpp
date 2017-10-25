#include "ComponentGizmoMesh.h"
#include "libraries/assimp/include/assimp/vector3.h"
#include <vector>
#include "GameObject.h"
#include "ComponentTransform.h"
#include "libraries/glew-2.0.0/include/GL/glew.h"

ComponentGizmoMesh::ComponentGizmoMesh(component_type t, bool act, GameObject *go) : Component(t, act, go) {}

ComponentGizmoMesh::~ComponentGizmoMesh() {}

void ComponentGizmoMesh::Update()
{
	aiVector3D position = aiVector3D(0.0f, 0.0f, 0.0f);

	for (std::vector<Component*>::iterator it = owner_go->components.begin(); it != owner_go->components.end(); it++)
	{
		if ((*it)->type == component_type::TRANSFORM)
		{
			position = ((ComponentTransform*)(*it))->position;
			break;
		}
	}

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);

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

	glColor3f(0.0f, 0.0f, 3.0f);
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

	glPopMatrix();
}

void ComponentGizmoMesh::Enable()
{
	active = true;
}

void ComponentGizmoMesh::Disable()
{
	active = false;
}