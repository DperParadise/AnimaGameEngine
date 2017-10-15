#ifndef __MODEL_H__
#define __MODEL_H__

#include "libraries/assimp/include/assimp/scene.h"
#include "libraries/assimp/include/assimp/Importer.hpp"
#include "libraries/glew-2.0.0/include/GL/glew.h"
#include "libraries\DevIL_Windows_SDK\include\IL\il.h"

class Model
{
	const aiScene *scene = nullptr;

public:

	Model();
	~Model();

	void Load(const char *file);
	void Draw();
	void Clear();

private:
	Assimp::Importer importer;
	float **vertex_array = nullptr;
	float **normal_array = nullptr;
	float **uv_array = nullptr;
	
	GLuint *textures = nullptr;
	ILuint *images = nullptr;
	ILubyte **image_data = nullptr;
};

#endif
