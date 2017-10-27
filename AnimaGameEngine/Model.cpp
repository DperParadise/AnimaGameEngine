#include "Model.h"
#include "libraries/assimp/include/assimp/postprocess.h"
#include "Globals.h"
#include "libraries/DevIL_Windows_SDK/include/IL/il.h"
#include "libraries/DevIL_Windows_SDK/include/IL/ilu.h"


Model::Model() {}
Model::~Model() {}

void Model::Load(const char *file)
{
	scene = importer.ReadFile(file,  /*aiPostProcessSteps::aiProcess_PreTransformVertices |*/ 
		aiPostProcessSteps::aiProcess_FlipUVs |
		aiPostProcessSteps::aiProcess_Triangulate /*| 
	aiProcessPreset_TargetRealtime_Fast*/);
	
	aiString texture_file;
	root_path = std::string(file);
	std::size_t found = root_path.find_last_of("/\\");
	char separator = root_path[found];
	root_path = root_path.substr(0, found);	
	root_path.push_back(separator);

	
	for (uint i = 0; i < scene->mNumMeshes; i++)
	{
		//load materials 	
		Material material;
		aiColor3D ambient;
		aiColor3D diffuse;
		aiColor3D specular;
		float shininess;

		uint mat_index = scene->mMeshes[i]->mMaterialIndex;	
		aiMaterial *mat = scene->mMaterials[mat_index];
		if (mat->Get(AI_MATKEY_COLOR_AMBIENT, ambient) == AI_SUCCESS)
		{
			material.ambient[0] = ambient.r;
			material.ambient[1] = ambient.g;
			material.ambient[2] = ambient.b;
		}
		if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse) == AI_SUCCESS)
		{
			material.diffuse[0] = diffuse.r;
			material.diffuse[1] = diffuse.g;
			material.diffuse[2] = diffuse.b;
		}
		if (mat->Get(AI_MATKEY_COLOR_SPECULAR, specular) == AI_SUCCESS)
		{
			material.specular[0] = specular.r;
			material.specular[1] = specular.g;
			material.specular[2] = specular.b;
		}
		if (mat->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
		{
			//material.shininess = shininess;
		}

		materials.push_back(material);

		//load textures
		if (mat->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE) != 0)
		{
			num_textures++;
			scene->mMaterials[mat_index]->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &texture_file);
			std::string texture_path(root_path);
			textureManager->GetInstance()->Load(aiString(texture_path.append(texture_file.data)));
		}
	}
	
	vertex_array = new float*[scene->mNumMeshes];
	normal_array = new float*[scene->mNumMeshes];
	if (num_textures != 0)
	{
		uv_array = new float*[num_textures];
		for (uint i = 0; i < num_textures; i++)
		{
			uv_array[i] = nullptr;
		}
	}

	for (uint i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[i];
		vertex_array[i] = new float[mesh->mNumFaces * 3 * 3];
		normal_array[i] = new float[mesh->mNumFaces * 3 * 3];
			
		uint mat_index = mesh->mMaterialIndex;
		if (scene->mMaterials[mat_index]->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE) != 0)
		{
			uv_array[i] = new float[mesh->mNumFaces * 3 * 2];
		}

		for (unsigned int j = 0; j < mesh->mNumFaces; j++)
		{
			const aiFace& face = mesh->mFaces[j];
			
			for (int k = 0; k<3; k++)
			{		
				if (uv_array != nullptr && uv_array[i] != nullptr)
				{
					aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[k]];
					memcpy(uv_array[i], &uv, sizeof(float) * 2);
					uv_array[i] += 2;
				}
				
				aiVector3D normal = mesh->mNormals[face.mIndices[k]];
				memcpy(normal_array[i], &normal, sizeof(float) * 3);
				normal_array[i] += 3;

				aiVector3D pos = mesh->mVertices[face.mIndices[k]];
				memcpy(vertex_array[i], &pos, sizeof(float) * 3);
				vertex_array[i] += 3;
			}
		}
		if (uv_array != nullptr && uv_array[i] != nullptr)
		{
			uv_array[i] -= mesh->mNumFaces * 3 * 2;
		}
		normal_array[i] -= mesh->mNumFaces * 3 * 3;
		vertex_array[i] -= mesh->mNumFaces * 3 * 3;	
	}

}

void Model::Draw()
{
	aiString texture_file;
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	if (num_textures != 0)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	for (uint i = 0; i < scene->mNumMeshes; i++)
	{	
		uint mat_index = scene->mMeshes[i]->mMaterialIndex;
		if (scene->mMaterials[mat_index]->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE) != 0)
		{
			scene->mMaterials[mat_index]->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &texture_file);
			std::string texture_path(root_path);
			unsigned int tex_id = textureManager->GetInstance()->Load(aiString(texture_path.append(texture_file.data)));
			glBindTexture(GL_TEXTURE_2D, tex_id);
			glTexCoordPointer(2, GL_FLOAT, 0, uv_array[i]);
		}

		glVertexPointer(3, GL_FLOAT, 0, vertex_array[i]);	
		glNormalPointer(GL_FLOAT, 0, normal_array[i]);

		glMaterialfv(GL_FRONT, GL_AMBIENT, materials[i].ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materials[i].diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, materials[i].specular);
		glMaterialf(GL_FRONT, GL_SHININESS, materials[i].shininess);

		uint num_verts = scene->mMeshes[i]->mNumFaces * 3;
		glDrawArrays(GL_TRIANGLES, 0, num_verts);	
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	if (num_textures != 0)
	{
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	glBindTexture(GL_TEXTURE_2D, 0);	
}

void Model::Clear()
{
	for (uint i = 0; i < scene->mNumMeshes; i++)
	{
		RELEASE_ARRAY(vertex_array[i]);
		RELEASE_ARRAY(normal_array[i]);
		if (num_textures != 0)
		{
			RELEASE_ARRAY(uv_array[i]);
		}
	}
	RELEASE_ARRAY(vertex_array);
	RELEASE_ARRAY(normal_array);
	if (num_textures != 0)
	{
		RELEASE_ARRAY(uv_array);
	}
	
}