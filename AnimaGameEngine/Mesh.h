#ifndef __MESH_H__
#define __MESH_H__

#include "libraries/glm/glm.hpp"
#include <string>
#include <vector>
#include "libraries/assimp/include/material.h"

struct aiMesh;
struct aiScene;

struct Vertex 
{
	glm::vec3 position;
	glm::vec2 texCoord;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

struct Texture 
{

	unsigned id;
	std::string name;
	std::string type;
};

class Mesh
{
public:
		Mesh();
		void LoadMesh(const aiScene *scene, const aiMesh *mesh, const std::string &texturePath);
		const std::vector<Texture>& GetTextures() const;
		unsigned int GetVAO() const;
		const auto& GetIndices() const;

private:
		unsigned VAO, VBO, EBO;
		std::vector<Vertex> vertices;
		std::vector<unsigned> indices;
		std::vector<Texture> textures;

		static std::vector<const Texture&> loadedTextures;

		void LoadVertices(const aiMesh *mesh);
		bool LoadIndices(const aiMesh *mesh);
		void SetVertexBuffers();
		void ClearVertexVectors();
		void LoadTextures(const aiScene *aiScene, const aiMesh *mesh, const std::string &texturePath);
		bool AlreadyLoaded(const std::string &name, Texture &outTexture) const;
		void LoadMaterialTextures(const aiMaterial *mat, aiTextureType type, const std::string &texturePath);
		unsigned CreateOpenGLTexture(const std::string texturePath);
		
		static void ClearLoadedTexturesVector();


};

#endif

