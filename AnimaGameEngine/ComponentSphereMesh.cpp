#include "ComponentSphereMesh.h"
#include "libraries/assimp/include/assimp/vector3.h"
#include <vector>
#include "GameObject.h"
#include "ComponentTransform.h"

ComponentSphereMesh::ComponentSphereMesh(component_type t, bool act, GameObject *go) : Component(t, act, go) 
{
	quadric = gluNewQuadric();
}

ComponentSphereMesh::~ComponentSphereMesh()
{
	gluDeleteQuadric(quadric);
}

void ComponentSphereMesh::Update()
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
	glRotated(90, 1, 0, 0);
	gluQuadricDrawStyle(quadric, GLU_LINE);
	gluSphere(quadric, radius, slices, stacks);
	glPopMatrix();
}

void ComponentSphereMesh::Enable()
{
	active = true;
}

void ComponentSphereMesh::Disable()
{
	active = false;
}

void ComponentSphereMesh::SetRadius(GLuint r)
{
	radius = r;
}

void ComponentSphereMesh::SetSlices(GLuint sl)
{
	slices = sl;
}

void ComponentSphereMesh::SetStacks(GLdouble st)
{
	stacks = st;
}
