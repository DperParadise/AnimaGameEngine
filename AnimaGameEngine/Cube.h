#ifndef __CUBE_H__
#define __CUBE_H__

#include <glew_2.0.0/GL/glew.h>
#include <devil/il.h>


#define NUM_VERTICES_COORD 6*2*3*3 
#define NUM_TEXTURE_COORD 6*2*3*2
#define NUM_TEXTURES 2
#define TEXTURE_WIDTH 64
#define TEXTURE_HEIGHT 64
#define TEXTURE_RGBA 4

class Cube
{
public:
	Cube();
	~Cube();

	void Init();
	void Clear();
	void Draw();

private:
	
	GLuint textures[NUM_TEXTURES];

	GLubyte checkeredImage[TEXTURE_HEIGHT][TEXTURE_WIDTH][TEXTURE_RGBA];
	ILuint image;
	ILubyte *image_data = nullptr;

	GLfloat vertices[NUM_VERTICES_COORD] = {
	0.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,

	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,

	1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f,

	1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f,

	0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 0.0f,

	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f,

	0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 0.0f,

	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 1.0f
};

	GLfloat colour[NUM_VERTICES_COORD] = {
	0.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,

	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,

	1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f,

	1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f,

	0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 0.0f,

	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f,

	0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 0.0f,

	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 1.0f
};

	GLfloat texture[NUM_TEXTURE_COORD] = {
	//Back face
	1.0f, 1.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,

	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,

	//Front face
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,

	0.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,

	//Left face
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,

	1.0f, 1.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,

	//Right face
	0.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,

	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,

	//Upper face
	
	0.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,

	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,

	//Bottom face
	0.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,

	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f
	};

};





#endif