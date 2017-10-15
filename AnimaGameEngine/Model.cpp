#include "Model.h"
#include "libraries/assimp/include/assimp/postprocess.h"
#include "Globals.h"
#include "libraries/glew-2.0.0/include/GL/glew.h"
#include "libraries\DevIL_Windows_SDK\include\IL\il.h"
#include "libraries\DevIL_Windows_SDK\include\IL\ilu.h"
#include <string>

Model::Model() {}
Model::~Model() {}

void Model::Load(const char *file)
{
	scene = importer.ReadFile(file, aiPostProcessSteps::aiProcess_PreTransformVertices | aiPostProcessSteps::aiProcess_FlipUVs/*aiProcessPreset_TargetRealtime_Fast*/);

	textures = new GLuint[scene->mNumMeshes];
	images = new ILuint[scene->mNumMeshes];
	image_data = new ILubyte*[scene->mNumMeshes];

	ilGenImages(scene->mNumMeshes, images);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(scene->mNumMeshes, textures);

	uint num_faces = 0;
	aiString path;

	for (uint i = 0; i < scene->mNumMeshes; i++)
	{
		uint mat_index = scene->mMeshes[i]->mMaterialIndex;
		scene->mMaterials[mat_index]->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &path);	
		std::string full_path = std::string(file);
		std::size_t found = full_path.find_last_of("/\\");
		char separator = full_path[found];
		full_path = full_path.substr(0, found);
		full_path.push_back(separator);
		full_path.append(path.data);

		ilBindImage(images[i]);
		ilLoad(IL_TGA, full_path.c_str());
		if (ilGetError() != IL_NO_ERROR)
			MYLOG(iluErrorString(ilGetError()));

		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		if (ilGetError() != IL_NO_ERROR)
			MYLOG(iluErrorString(ilGetError()));

		uint img_widht = ilGetInteger(IL_IMAGE_WIDTH);
		uint img_height = ilGetInteger(IL_IMAGE_HEIGHT);
		image_data[i] = ilGetData();
		if (ilGetError() != IL_NO_ERROR)
			MYLOG(iluErrorString(ilGetError()));

		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_widht, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data[i]);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	}

	for (uint i = 0; i < scene->mNumMeshes; i++)
	{		
		iluDeleteImage(images[i]);
	}

	vertex_array = new float*[scene->mNumMeshes];
	normal_array = new float*[scene->mNumMeshes];
	uv_array = new float*[scene->mNumMeshes];

	for (uint i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[i];
		vertex_array[i] = new float[mesh->mNumFaces * 3 * 3];
		normal_array[i] = new float[mesh->mNumFaces * 3 * 3];
		uv_array[i] = new float[mesh->mNumFaces * 3 * 2];

		for (unsigned int j = 0; j < mesh->mNumFaces; j++)
		{
			const aiFace& face = mesh->mFaces[j];

			for (int k = 0; k<3; k++)
			{
				aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[k]];
				memcpy(uv_array[i], &uv, sizeof(float) * 2);
				uv_array[i] += 2;

				aiVector3D normal = mesh->mNormals[face.mIndices[k]];
				memcpy(normal_array[i], &normal, sizeof(float) * 3);
				normal_array[i] += 3;

				aiVector3D pos = mesh->mVertices[face.mIndices[k]];
				memcpy(vertex_array[i], &pos, sizeof(float) * 3);
				vertex_array[i] += 3;
			}
		}

		uv_array[i] -= mesh->mNumFaces * 3 * 2;
		normal_array[i] -= mesh->mNumFaces * 3 * 3;
		vertex_array[i] -= mesh->mNumFaces * 3 * 3;	
	}

}

void Model::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	for (uint i = 0; i < scene->mNumMeshes; i++)
	{
		glBindTexture(GL_TEXTURE_2D, textures[i]);
	
		glVertexPointer(3, GL_FLOAT, 0, vertex_array[i]);	
		glNormalPointer(GL_FLOAT, 0, normal_array[i]);		
		glTexCoordPointer(2, GL_FLOAT, 0, uv_array[i]);

		uint num_verts = scene->mMeshes[i]->mNumFaces * 3;
		glDrawArrays(GL_TRIANGLES, 0, num_verts);	
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Model::Clear()
{
	for (uint i = 0; i < scene->mNumMeshes; i++)
	{
		RELEASE_ARRAY(vertex_array[i]);
		RELEASE_ARRAY(normal_array[i]);
		RELEASE_ARRAY(uv_array[i]);
	}
	RELEASE_ARRAY(vertex_array);
	RELEASE_ARRAY(normal_array);
	RELEASE_ARRAY(uv_array);
	RELEASE_ARRAY(image_data);

	RELEASE_ARRAY(textures);
	RELEASE_ARRAY(images);
}