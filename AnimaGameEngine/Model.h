#ifndef __MODEL_H__
#define __MODEL_H__

#include "libraries/assimp/include/assimp/scene.h"
#include "libraries/assimp/include/assimp/Importer.hpp"
#include "libraries/glew-2.0.0/include/GL/glew.h"
#include "libraries\DevIL_Windows_SDK\include\IL\il.h"
#include "TextureManager.h"
#include <string>
#include <vector>

class Model
{
	const aiScene *scene = nullptr;

public:

	Model();
	~Model();

	void Load(const char *file);
	void Draw();
	void Clear();

	struct Material
	{
		GLfloat ambient[4] = {0.0f,0.0f,0.f,1.0f};
		GLfloat diffuse[4] = { 0.0f,0.0f,0.f,1.0f };
		GLfloat specular[4] = { 0.0f,0.0f,0.f,1.0f };
		GLfloat shininess = 32.0f;
	};

	std::vector<Material> materials;

private:
	Assimp::Importer importer;
	float **vertex_array = nullptr;
	float **normal_array = nullptr;
	float **uv_array = nullptr;
	
	TextureManager *textureManager = nullptr;
	std::string root_path;
	unsigned int num_textures = 0;

};

#endif
