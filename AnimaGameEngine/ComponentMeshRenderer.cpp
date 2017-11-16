#include "ComponentMeshRenderer.h"
#include <vector>
#include "GameObject.h"
#include "ComponentMesh.h"
#include "libraries/glew-2.0.0/include/GL/glew.h"
#include "Globals.h"
#include "ComponentMaterial.h"
#include "TextureManager.h"

ComponentMeshRenderer::ComponentMeshRenderer(ComponentMesh *mesh_comp, component_type t, bool act, GameObject *go) : owner_mesh(mesh_comp), Component(t, act, go){}

ComponentMeshRenderer::~ComponentMeshRenderer() {}

void ComponentMeshRenderer::Update(float dt)
{
	if (active)
	{
		//Draw mesh
		
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		if (owner_mesh->uv_array)
		{
			uint tex_id = TextureManager::GetInstance()->Load(owner_mesh->mesh_mat->diffuse_texture);
			glBindTexture(GL_TEXTURE_2D, tex_id);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, 0, owner_mesh->uv_array);		
		}
		
		glVertexPointer(3, GL_FLOAT, 0, owner_mesh->vertex_array);
		glNormalPointer(GL_FLOAT, 0, owner_mesh->normal_array);
	
		glMaterialfv(GL_FRONT, GL_AMBIENT, owner_mesh->mesh_mat->material.ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, owner_mesh->mesh_mat->material.diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, owner_mesh->mesh_mat->material.specular);
		glMaterialf(GL_FRONT, GL_SHININESS, owner_mesh->mesh_mat->material.shininess);

		/*MYLOG("ambient(%f,%f,%f,%f)  diffuse(%f,%f,%f,%f)  specular(%f,%f,%f,%f)  shinniness(%f)", owner_mesh->mesh_mat->material.ambient[0],
			owner_mesh->mesh_mat->material.ambient[1], owner_mesh->mesh_mat->material.ambient[2], owner_mesh->mesh_mat->material.ambient[3],
			owner_mesh->mesh_mat->material.diffuse[0], owner_mesh->mesh_mat->material.diffuse[1], owner_mesh->mesh_mat->material.diffuse[2],
			owner_mesh->mesh_mat->material.diffuse[3], owner_mesh->mesh_mat->material.specular[0], owner_mesh->mesh_mat->material.specular[1],
			owner_mesh->mesh_mat->material.specular[2], owner_mesh->mesh_mat->material.specular[3], owner_mesh->mesh_mat->material.shininess);*/
		
		//Apply world transform
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			
		glTranslatef(owner_go->transform.world_position.x, owner_go->transform.world_position.y, owner_go->transform.world_position.z);
			
		aiMatrix3x3 rot = owner_go->transform.world_rotation.GetMatrix(); //matrix in row-major order. Transpose before feeding openGL 
		GLfloat rot4x4[16] = {		0.0f, 0.0f, 0.0f, 0.0f,
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

		glScalef(owner_go->transform.world_scale.x, owner_go->transform.world_scale.y, owner_go->transform.world_scale.z);
		
		glDrawArrays(GL_TRIANGLES, 0, owner_mesh->num_vertices);

		glPopMatrix();

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);	

		if (owner_mesh->uv_array)
		{
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void ComponentMeshRenderer::Enable()
{
	active = true;
}

void ComponentMeshRenderer::Disable()
{
	active = false;
}