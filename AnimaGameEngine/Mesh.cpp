#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glew_2.0.0/GL/glew.h>
#include <devil/il.h>
#include "Globals.h"
#include <cstring>
#include "Skeleton.h"

std::vector<Texture> Mesh::loadedTextures = std::vector<Texture>();

Vertex::Vertex()
{
	memset(bones, 0, 4 * sizeof(unsigned int));
	memset(weights, 0.0f, 4 * sizeof(float));
}

Mesh::Mesh(const aiScene *scene, const aiMesh *aiMesh, const std::string &texturePath, Skeleton *skeleton)
{
	LoadMesh(scene, aiMesh, texturePath, skeleton);
}

void Mesh::LoadMesh(const aiScene *scene, const aiMesh *aiMesh, const std::string &texturePath, Skeleton *skeleton)
{
	LoadVertices(aiMesh);
	LoadIndices(aiMesh);
	
	if(skeleton)
		SetVertexWeightsMD5(aiMesh, skeleton);

	SetVertexBuffers();
	ClearVertexVectors();
	LoadTextures(scene, aiMesh, texturePath);
	LoadMaterial(scene, aiMesh);
	ClearLoadedTexturesVector();
}

const std::vector<Texture>& Mesh::GetTextures() const
{
	return textures;
}

unsigned int Mesh::GetVAO() const
{
	return VAO;
}

unsigned int Mesh::GetEBO() const
{
	return EBO;
}

unsigned int Mesh::GetVBO() const
{
	return VBO;
}

std::vector<Vertex>& Mesh::GetVertices()
{
	return vertices;
}

const std::vector<unsigned int> & Mesh::GetIndices() const
{
	return indices;
}

void Mesh::LoadVertices(const aiMesh * mesh)
{
	const aiVector3D zero3D(0.0f, 0.0f, 0.0f);
	vertices.reserve(mesh->mNumVertices);

	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		const aiVector3D pos = mesh->mVertices[i];
		const aiVector3D texCoord = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : zero3D;
		const aiVector3D normal = mesh->HasNormals() ? mesh->mNormals[i] : zero3D;
		const aiVector3D tangent = mesh->HasTangentsAndBitangents() ? mesh->mTangents[i] : zero3D;
		const aiVector3D bitangent = mesh->HasTangentsAndBitangents() ? mesh->mBitangents[i] : zero3D;

		Vertex vertex;
		vertex.position = glm::vec3(pos.x, pos.y, pos.z);
		vertex.texCoord = glm::vec2(texCoord.x, texCoord.y);
		vertex.normal = glm::vec3(normal.x, normal.y, normal.z);
		vertex.tangent = glm::vec3(tangent.x, tangent.y, tangent.z);
		vertex.bitangent = glm::vec3(bitangent.x, bitangent.y, bitangent.z);

		vertices.push_back(vertex);
	}
}

bool Mesh::LoadIndices(const aiMesh * mesh)
{
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		const aiFace &face = mesh->mFaces[i];
		if (face.mNumIndices != 3)
		{
			return false;
		}
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}
	return true;
}

void Mesh::SetVertexBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	
	//vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	//texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

	//normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	//tangents
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

	//bitangents
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

	//boneIds
	glEnableVertexAttribArray(5);
	glVertexAttribIPointer(5, 4, GL_UNSIGNED_INT, sizeof(Vertex), (void*)offsetof(Vertex, bones));
	
	//weights
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, weights));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::ClearVertexVectors()
{
	vertices.clear();
}

void Mesh::LoadTextures(const aiScene *aiScene, const aiMesh *mesh, const std::string &texturePath)
{
	aiMaterial *material = aiScene->mMaterials[mesh->mMaterialIndex];

	LoadMaterialTextures(material, aiTextureType::aiTextureType_DIFFUSE, texturePath);
	LoadMaterialTextures(material, aiTextureType::aiTextureType_SPECULAR, texturePath);
	LoadMaterialTextures(material, aiTextureType::aiTextureType_NORMALS, texturePath);
	LoadMaterialTextures(material, aiTextureType::aiTextureType_HEIGHT, texturePath);
}


bool Mesh::AlreadyLoaded(const std::string &name, Texture& outTexture) const
{
	for (const Texture& texture : loadedTextures)
	{
		if (texture.name == name)
		{
			outTexture.id = texture.id;
			outTexture.name = texture.name;
			outTexture.type = texture.type;
		}
		return true;
	}
	return false;
}

void Mesh::LoadMaterialTextures(const aiMaterial *mat, aiTextureType type, const std::string &texturePath)
{
	for (int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString name;
		mat->GetTexture(type, i, &name);

		Texture texture;
		if(AlreadyLoaded(std::string(name.data), texture))
		{
			textures.push_back(texture);
		}
		else
		{
			unsigned textureId = CreateOpenGLTexture(texturePath + "/" + std::string(name.data));
			texture.id = textureId;
			texture.name = std::string(name.data);

			switch (type)
			{
			case aiTextureType::aiTextureType_DIFFUSE:
				texture.type = "texture_diffuse";
				break;
			case aiTextureType::aiTextureType_SPECULAR:
				texture.type = "texture_specular";
				break;
			case aiTextureType::aiTextureType_NORMALS:
				texture.type = "texture_normals";
				break;
			case aiTextureType::aiTextureType_HEIGHT:
				texture.type = "texture_height";
				break;
			}

			textures.push_back(texture);
			loadedTextures.push_back(textures.back());
		}
	}
}

unsigned Mesh::CreateOpenGLTexture(const std::string texturePath)
{
	ILuint imageId;
	ilGenImages(1, &imageId);
	ilBindImage(imageId);

	if (ilLoadImage(texturePath.data()))
	{
		GLuint textureId = 0;
		glGenTextures(1, &textureId);

		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		int format = GL_RGB;
		int internalFormat = GL_RGB;

		switch (ilGetInteger(IL_IMAGE_FORMAT))
		{
		case IL_RGB:
			format = GL_RGB;
			internalFormat = GL_RGB;
			break;

		case IL_RGBA:
			format = GL_RGBA;
			internalFormat = GL_RGBA;
			break;

		case IL_BGR:
			format = GL_BGR;
			internalFormat = GL_RGB;
			break;

		case IL_BGRA:
			format = GL_BGRA;
			internalFormat = GL_RGBA;
			break;

		default:
			assert(false);
		}

		ILubyte* data = ilGetData();
		int width = ilGetInteger(IL_IMAGE_WIDTH);
		int height = ilGetInteger(IL_IMAGE_HEIGHT);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		ilDeleteImages(1, &imageId);

		glBindTexture(GL_TEXTURE_2D, 0);

		if (GL_NO_ERROR != glGetError())
		{
			MYLOG("ERROR: Mesh: OpenGL error loading texture : code = %d", glGetError())	
			return 0;
		}

		return textureId;
	}
	MYLOG("ERROR: Mesh: DevIL error loading image")
	return 0;
}

/*
* This method will be called after all meshes have been loaded for non animated models
*/
void Mesh::ClearLoadedTexturesVector()
{
	loadedTextures.clear();
}

void Mesh::LoadMaterial(const aiScene * aiScene, const aiMesh * mesh)
{
	unsigned materialIndex = mesh->mMaterialIndex;
	aiMaterial *material = aiScene->mMaterials[materialIndex];

	aiColor3D color;
	float shininess;

	if (material->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS)
	{
		this->material.ambient.r = color.r;
		this->material.ambient.g = color.g;
		this->material.ambient.b = color.b;
	}

	if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
	{
		this->material.diffuse.r = color.r;
		this->material.diffuse.g = color.g;
		this->material.diffuse.b = color.b;
	}

	if (material->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
	{
		this->material.specular.r = color.r;
		this->material.specular.g = color.g;
		this->material.specular.b = color.b;
	}

	if (material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
		this->material.shininess = shininess;

}

void Mesh::SetVertexWeightsMD5(const aiMesh *aiMesh, const Skeleton *skeleton)
{
	if (aiMesh->HasBones())
	{
		std::vector<int> visitedVertices(aiMesh->mNumVertices, -1);

		for (uint i = 0; i < aiMesh->mNumBones; ++i)
		{
			aiBone *aiBone = aiMesh->mBones[i];
			aiVertexWeight *vertexWeights = aiBone->mWeights;
			for (uint j = 0; j < aiBone->mNumWeights; ++j)
			{
				unsigned int vertexId = vertexWeights[j].mVertexId;
				float weight = vertexWeights[j].mWeight;

				unsigned int boneIndex = 0;
				int found = visitedVertices[vertexId];

				if (found == -1)
				{
					visitedVertices[vertexId] = 0;
				}

				boneIndex = visitedVertices[vertexId];
				
				//TODO: Decide how to manage a bigger number of bones affecting a vertex
				if (boneIndex > 3)
				{
					MYLOG("Error: Number of bones influencing bones is bigger than 4")
					continue;
				}

				visitedVertices[vertexId] += 1;

				int boneId = skeleton->FindBoneInSkeleton(aiBone->mName.C_Str());
				if (boneId == -1)
				{
					MYLOG("Error: bone %s not found", aiBone->mName.C_Str())
				}

				vertices[vertexId].bones[boneIndex] = boneId;
				vertices[vertexId].weights[boneIndex] = weight;
			}
		}
	}
}

const Material & Mesh::GetMaterial() const
{
	return material;
}

void Mesh::AddBoneName(const std::string & boneName)
{
	bones.push_back(boneName);
}

const std::vector<std::string>& Mesh::GetBones() const
{
	return bones;
}
