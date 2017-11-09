#include "ComponentLoadedMesh.h"
#include "Globals.h"
#include "libraries/assimp/include/assimp/scene.h"
#include "libraries/glew-2.0.0/include/GL/glew.h"
#include <vector>

ComponentLoadedMesh::ComponentLoadedMesh(component_type t, bool act, GameObject *go, aiMesh *mesh) : Component(t, act, go)
{
	Load(mesh);
}

ComponentLoadedMesh::~ComponentLoadedMesh()
{
	RELEASE_ARRAY(vertex_array);
	RELEASE_ARRAY(normal_array);
	RELEASE_ARRAY(uv_array);
}

void ComponentLoadedMesh::Update()
{
	//if (!active)
	//{	
	//	return;
	//}

	////draw the mesh
	//aiVector3D position = aiVector3D(0.0f,0.0f,0.0f);

	//for (std::vector<Component*>::iterator it = owner_go->components.begin(); it != owner_go->components.end(); it++)
	//{
	//	if ((*it)->type == component_type::TRANSFORM)
	//	{
	//		position = ((ComponentTransform*)(*it))->position;
	//		break;
	//	}
	//}

	//ComponentTexture *comp_tex = nullptr;
	//for (std::vector<Component*>::iterator it = owner_go->components.begin(); it != owner_go->components.end(); it++)
	//{
	//	if ((*it)->type == component_type::TEXTURE)
	//	{
	//		comp_tex = ((ComponentTexture*)(*it));
	//		break;
	//	}
	//}

	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_NORMAL_ARRAY);

	//if (comp_tex->HasTexture())
	//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//
	//glPushMatrix();
	//glTranslatef(position.x, position.y, position.z);


	////int mat_count = 0;
	//for (uint i = 0; i < num_meshes; i++)
	//{
	//	glVertexPointer(3, GL_FLOAT, 0, vertex_array[i]);
	//	glNormalPointer(GL_FLOAT, 0, normal_array[i]);

	//	ComponentMaterial::Material mat;
	//	
	//	for (std::vector<Component*>::iterator it = owner_go->components.begin(); it != owner_go->components.end(); it++)
	//	{
	//		if ((*it)->type == component_type::MATERIAL)
	//		{
	//			mat = ((ComponentMaterial*)(*it))->materials[i];
	//			break;
	//		}
	//	}

	//	if (comp_tex->HasTexture())
	//	{
	//		float *uv_array = comp_tex->uv_array[i];
	//		std::string texture_path = comp_tex->texture_paths[i];
	//		uint tex_id = TextureManager::GetInstance()->Load(aiString(texture_path));
	//		
	//		glTexCoordPointer(2, GL_FLOAT, 0, uv_array);
	//		glBindTexture(GL_TEXTURE_2D, tex_id);
	//	}

	//	uint num_verts = num_vertices[i];
	//	glMaterialfv(GL_FRONT, GL_AMBIENT, mat.ambient);
	//	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat.diffuse);
	//	glMaterialfv(GL_FRONT, GL_SPECULAR, mat.specular);
	//	glMaterialf(GL_FRONT, GL_SHININESS, mat.shininess);
	//	glDrawArrays(GL_TRIANGLES, 0, num_verts);

	//	glBindTexture(GL_TEXTURE_2D, 0);

	//	/*MYLOG("%d  amb = (%f, %f, %f)   diff = (%f,%f,%f)   spec = (%f,%f,%f)   shin = %f", mat_count, mat.ambient[0], mat.ambient[1], mat.ambient[2],
	//		mat.diffuse[0], mat.diffuse[1], mat.diffuse[2],
	//		mat.specular[0], mat.specular[1], mat.specular[2],
	//		mat.shininess);

	//	mat_count++;*/
	//}

	//glPopMatrix();

	//glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_NORMAL_ARRAY);	

	//if (comp_tex != nullptr)
	//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void ComponentLoadedMesh::Enable()
{
	active = true;
}

void ComponentLoadedMesh::Disable()
{
	active = false;
}

void ComponentLoadedMesh::Load(aiMesh *mesh)
{	
	if (!mesh->HasFaces())
	{
		MYLOG("mesh does not contain faces");
		return;
	}

	num_vertices = mesh->mNumFaces * 3;
	vertex_array = new float[num_vertices * 3];
		
	if (mesh->HasNormals())
	{
		normal_array = new float[mesh->mNumFaces * 3 * 3];
	}

	if (mesh->HasTextureCoords(0))
	{
		uv_array = new float[num_vertices * 2];
	}

	for (unsigned int j = 0; j < mesh->mNumFaces; j++)
	{
		const aiFace& face = mesh->mFaces[j];

		for (int k = 0; k<3; k++)
		{			
			aiVector3D pos = mesh->mVertices[face.mIndices[k]];
			memcpy(vertex_array, &pos, sizeof(float) * 3);
			vertex_array += 3;

			if (mesh->HasNormals())
			{
				aiVector3D normal = mesh->mNormals[face.mIndices[k]];
				memcpy(normal_array, &normal, sizeof(float) * 3);
				normal_array += 3;
			}

			if (mesh->HasTextureCoords(0))
			{
				aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[k]];
				memcpy(uv_array, &uv, sizeof(float) * 2);
				uv_array += 2;
			}
		}
	}

	vertex_array -= num_vertices * 3;
	
	if (mesh->HasNormals())
	{
		normal_array -= num_vertices * 3;
	}

	if (mesh->HasTextureCoords(0))
	{
		uv_array -= num_vertices * 2;
	}
}
