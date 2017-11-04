#include "ComponentLoadedMesh.h"
#include "libraries/assimp/include/assimp/vector3.h"
#include "libraries/assimp/include/assimp/postprocess.h"
#include <vector>
#include "GameObject.h"
#include "ComponentTransform.h"
#include "Importer.h"
#include "Globals.h"
#include "libraries/glew-2.0.0/include/GL/glew.h"
#include "ComponentMaterial.h"
#include "ComponentTexture.h"
#include "TextureManager.h"

ComponentLoadedMesh::ComponentLoadedMesh(const char *file, component_type t, bool act, GameObject *go) : Component(t, act, go)
{
	if (file == nullptr)
	{
		MYLOG("Error invalid nullptr file: %s");
		return;
	}

	LoadMesh(file);
}

ComponentLoadedMesh::~ComponentLoadedMesh()
{
	for (uint i = 0; i < num_meshes; i++)
	{
		RELEASE_ARRAY(vertex_array[i]);
		RELEASE_ARRAY(normal_array[i]);
	}
	RELEASE_ARRAY(vertex_array);
	RELEASE_ARRAY(normal_array);
	RELEASE_ARRAY(num_vertices);
}

void ComponentLoadedMesh::Update()
{
	if (!active)
	{	
		return;
	}

	//draw the mesh
	aiVector3D position = aiVector3D(0.0f,0.0f,0.0f);

	for (std::vector<Component*>::iterator it = owner_go->components.begin(); it != owner_go->components.end(); it++)
	{
		if ((*it)->type == component_type::TRANSFORM)
		{
			position = ((ComponentTransform*)(*it))->position;
			break;
		}
	}

	ComponentTexture *comp_tex = nullptr;
	for (std::vector<Component*>::iterator it = owner_go->components.begin(); it != owner_go->components.end(); it++)
	{
		if ((*it)->type == component_type::TEXTURE)
		{
			comp_tex = ((ComponentTexture*)(*it));
			break;
		}
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	if (comp_tex->HasTexture())
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);


	//int mat_count = 0;
	for (uint i = 0; i < num_meshes; i++)
	{
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
			uint tex_id = comp_tex->texture_manager->GetInstance()->Load(aiString(texture_path));
			
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

		/*MYLOG("%d  amb = (%f, %f, %f)   diff = (%f,%f,%f)   spec = (%f,%f,%f)   shin = %f", mat_count, mat.ambient[0], mat.ambient[1], mat.ambient[2],
			mat.diffuse[0], mat.diffuse[1], mat.diffuse[2],
			mat.specular[0], mat.specular[1], mat.specular[2],
			mat.shininess);

		mat_count++;*/
	}

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);	

	if (comp_tex != nullptr)
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void ComponentLoadedMesh::Enable()
{
	active = true;
}

void ComponentLoadedMesh::Disable()
{
	active = false;
}

void ComponentLoadedMesh::LoadMesh(const char *file)
{	

	scene = importer->GetInstance()->ReadFile(file,  aiPostProcessSteps::aiProcess_PreTransformVertices |
		aiPostProcessSteps::aiProcess_FlipUVs |	aiPostProcessSteps::aiProcess_Triangulate /*|aiProcessPreset_TargetRealtime_Fast*/);

	if (scene == NULL)
	{
		MYLOG("Error importing scene: %s", importer->GetInstance()->GetErrorString());
		return;
	}

	num_meshes = scene->mNumMeshes;
	if (num_meshes == 0)
	{
		MYLOG("No meshes in the imported scene");
		return;
	}

	vertex_array = new float*[num_meshes];
	normal_array = new float*[num_meshes];
	num_vertices = new uint[num_meshes];
	
	for (uint i = 0; i < num_meshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[i];
		vertex_array[i] = new float[mesh->mNumFaces * 3 * 3];
		normal_array[i] = new float[mesh->mNumFaces * 3 * 3];
		num_vertices[i] = mesh->mNumFaces * 3;

		for (unsigned int j = 0; j < mesh->mNumFaces; j++)
		{
			const aiFace& face = mesh->mFaces[j];

			for (int k = 0; k<3; k++)
			{			
				aiVector3D normal = mesh->mNormals[face.mIndices[k]];
				memcpy(normal_array[i], &normal, sizeof(float) * 3);
				normal_array[i] += 3;

				aiVector3D pos = mesh->mVertices[face.mIndices[k]];
				memcpy(vertex_array[i], &pos, sizeof(float) * 3);
				vertex_array[i] += 3;
			}
		}		
		normal_array[i] -= mesh->mNumFaces * 3 * 3;
		vertex_array[i] -= mesh->mNumFaces * 3 * 3;
	}
}
