#include "ComponentMeshRenderer.h"
#include <vector>
#include "GameObject.h"
#include "ComponentLoadedMesh.h"
#include "libraries/glew-2.0.0/include/GL/glew.h"
#include "Globals.h"

ComponentMeshRenderer::ComponentMeshRenderer(ComponentLoadedMesh *mesh_comp, component_type t, bool act, GameObject *go) : owner_mesh(mesh_comp), Component(t, act, go) {}

ComponentMeshRenderer::~ComponentMeshRenderer() {}

void ComponentMeshRenderer::Update()
{
	if (active)
	{
		//Draw mesh
		
		/*glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		if (comp_tex->HasTexture())
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		
		glPushMatrix();
		glTranslatef(position.x, position.y, position.z);


		
		glVertexPointer(3, GL_FLOAT, 0, vertex_array[i]);
		glNormalPointer(GL_FLOAT, 0, normal_array[i]);

		ComponentMaterial::Material mat;
			
		for (std::vector<Component*>::iterator it = owner_go->components.begin(); it != owner_go->components.end(); it++)
		{
			if ((*it)->type == component_type::MATERIAL)
			{
				mat = ((ComponentMaterial*)(*it))->materials[i];
				break;
			}
		}

		if (comp_tex->HasTexture())
		{
			float *uv_array = comp_tex->uv_array[i];
			std::string texture_path = comp_tex->texture_paths[i];
			uint tex_id = TextureManager::GetInstance()->Load(aiString(texture_path));
				
			glTexCoordPointer(2, GL_FLOAT, 0, uv_array);
			glBindTexture(GL_TEXTURE_2D, tex_id);
		}

		uint num_verts = num_vertices[i];
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat.ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat.diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat.specular);
		glMaterialf(GL_FRONT, GL_SHININESS, mat.shininess);
		glDrawArrays(GL_TRIANGLES, 0, num_verts);

		glBindTexture(GL_TEXTURE_2D, 0);


		

		glPopMatrix();

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);	

		if (comp_tex != nullptr)
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);*/
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