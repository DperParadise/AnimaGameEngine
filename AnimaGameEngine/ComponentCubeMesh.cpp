#include "ComponentCubeMesh.h"
#include "libraries/assimp/include/assimp/vector3.h"
#include <vector>
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"

ComponentCubeMesh::ComponentCubeMesh(component_type t, bool act, GameObject *go) : Component(t, act, go){}

ComponentCubeMesh::~ComponentCubeMesh() {}

void ComponentCubeMesh::Update()
{
	if (!active)
		return;

	//draw the mesh
	aiVector3D position = aiVector3D(0.0f, 0.0f, 0.0f);
	ComponentMaterial::Material mat;
	bool has_mat = false;

	for (std::vector<Component*>::iterator it = owner_go->components.begin(); it != owner_go->components.end(); it++)
	{
		if ((*it)->type == component_type::TRANSFORM)
		{
			position = ((ComponentTransform*)(*it))->local_position;
			break;
		}
	}

	/*for (std::vector<Component*>::iterator it = owner_go->components.begin(); it != owner_go->components.end(); it++)
	{
		if ((*it)->type == component_type::MATERIAL)
		{
			mat = ((ComponentMaterial*)(*it))->materials[0];
			has_mat = true;
			break;
		}
	}*/
	
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);

	if (!has_mat)
	{
		glEnable(GL_COLOR_MATERIAL);
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat.ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat.diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat.specular);
		glMaterialf(GL_FRONT, GL_SHININESS, mat.shininess);
	}

	glDrawArrays(GL_TRIANGLES, 0, NUM_VERT);
	glPopMatrix();
	glDisableClientState(GL_VERTEX_ARRAY);
	
	if (!has_mat)
		glDisable(GL_COLOR_MATERIAL);
}

void ComponentCubeMesh::Enable()
{
	active = true;
}

void ComponentCubeMesh::Disable()
{
	active = false;
}