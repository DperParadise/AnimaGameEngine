#ifndef __MESH_H__
#define __MESH_H__

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <assimp/material.h>

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

struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

class Mesh
{
public:
		Mesh();
		void LoadMesh(const aiScene *scene, const aiMesh *mesh, const std::string &texturePath);
		const std::vector<Texture>& GetTextures() const;
		unsigned int GetVAO() const;
		unsigned int GetEBO() const;

		const std::vector<unsigned int>& GetIndices() const;
		const Material &GetMaterial() const;

		void AddBoneName(const std::string &boneName);
		const std::vector<std::string>& GetBones() const;

private:
	unsigned VAO, VBO, EBO;
		std::vector<Vertex> vertices;
		std::vector<unsigned> indices;
		std::vector<Texture> textures;
		Material material;
		std::vector<std::string> bones;

		static std::vector<Texture> loadedTextures;

		void LoadVertices(const aiMesh *mesh);
		bool LoadIndices(const aiMesh *mesh);
		void SetVertexBuffers();
		void ClearVertexVectors();
		void LoadTextures(const aiScene *aiScene, const aiMesh *mesh, const std::string &texturePath);
		bool AlreadyLoaded(const std::string &name, Texture &outTexture) const;
		void LoadMaterialTextures(const aiMaterial *mat, aiTextureType type, const std::string &texturePath);
		unsigned CreateOpenGLTexture(const std::string texturePath);
		static void ClearLoadedTexturesVector();
		void LoadMaterial(const aiScene *aiScene, const aiMesh *mesh);
		



};

#endif

