#include "ComponentLoadedMesh.h"
#include "libraries/assimp/include/assimp/vector3.h"
#include "libraries/assimp/include/assimp/postprocess.h"
#include <vector>
#include "GameObject.h"
#include "ComponentTransform.h"
#include "Importer.h"
#include "Globals.h"
#include "libraries/glew-2.0.0/include/GL/glew.h"

ComponentLoadedMesh::ComponentLoadedMesh(const char *file, component_type t, bool act, GameObject *go) : Component(t, act, go)
{
	LoadMesh(file);
}

ComponentLoadedMesh::~ComponentLoadedMesh()
{
	for (uint i = 0; i < scene->mNumMeshes; i++)
	{
		RELEASE_ARRAY(vertex_array[i]);
		RELEASE_ARRAY(normal_array[i]);
	}
	RELEASE_ARRAY(vertex_array);
	RELEASE_ARRAY(normal_array);
}

void ComponentLoadedMesh::Update()
{
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

	aiString texture_file;
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	for (uint i = 0; i < scene->mNumMeshes; i++)
	{
		glVertexPointer(3, GL_FLOAT, 0, vertex_array[i]);
		glNormalPointer(GL_FLOAT, 0, normal_array[i]);	
		uint num_verts = scene->mMeshes[i]->mNumFaces * 3;
		glDrawArrays(GL_TRIANGLES, 0, num_verts);
	}
	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);	
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
	scene = importer->GetInstance()->ReadFile(file,  /*aiPostProcessSteps::aiProcess_PreTransformVertices |*/
		aiPostProcessSteps::aiProcess_FlipUVs |	aiPostProcessSteps::aiProcess_Triangulate /*|aiProcessPreset_TargetRealtime_Fast*/);

	if (scene == NULL)
	{
		MYLOG("Error importing scene: %s", importer->GetInstance()->GetErrorString());
		return;
	}

	if (scene->mNumMeshes == 0)
	{
		MYLOG("No meshes in the imported object");
		return;
	}

	vertex_array = new float*[scene->mNumMeshes];
	normal_array = new float*[scene->mNumMeshes];
	
	for (uint i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[i];
		vertex_array[i] = new float[mesh->mNumFaces * 3 * 3];
		normal_array[i] = new float[mesh->mNumFaces * 3 * 3];

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
