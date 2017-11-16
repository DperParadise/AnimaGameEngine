#include "ComponentMesh.h"
#include "Globals.h"
#include "libraries/assimp/include/assimp/scene.h"
#include "libraries/glew-2.0.0/include/GL/glew.h"
#include <vector>

ComponentMesh::ComponentMesh(ComponentMaterial *mat, component_type t, bool act, GameObject *go, aiMesh *mesh) : mesh_mat(mat), Component(t, act, go)
{
	Load(mesh);
}

ComponentMesh::ComponentMesh(ComponentMaterial *mat, component_type t, bool act, GameObject *go, 
	float *vertices, 
	float *normals, 
	float *uv) : vertex_array(vertices), normal_array(normals), uv_array(uv), mesh_mat(mat), Component(t, act, go){}

ComponentMesh::~ComponentMesh()
{
	RELEASE_ARRAY(vertex_array);
	RELEASE_ARRAY(normal_array);
	RELEASE_ARRAY(uv_array);
}

void ComponentMesh::Update(float dt){}

void ComponentMesh::Enable()
{
	active = true;
}

void ComponentMesh::Disable()
{
	active = false;
}

void ComponentMesh::Load(aiMesh *mesh)
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