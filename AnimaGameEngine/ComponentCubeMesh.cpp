#include "ComponentCubeMesh.h"
#include "libraries/assimp/include/assimp/vector3.h"
#include <vector>
#include "GameObject.h"
#include "ComponentTransform.h"

ComponentCubeMesh::ComponentCubeMesh(component_type t, bool act, GameObject *go) : Component(t, act, go){}

ComponentCubeMesh::~ComponentCubeMesh() {}

void ComponentCubeMesh::Update()
{
	//draw the mesh
	aiVector3D position;

	for (std::vector<Component*>::iterator it = owner_go->components.begin(); it != owner_go->components.end(); it++)
	{
		if ((*it)->type == component_type::TRANSFORM)
			position = ((ComponentTransform*)(*it))->position;	
	}

	glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	glDrawArrays(GL_TRIANGLES, 0, NUM_VERT);
	glPopMatrix();
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ComponentCubeMesh::Enable()
{
	active = true;
}

void ComponentCubeMesh::Disable()
{
	active = false;
}