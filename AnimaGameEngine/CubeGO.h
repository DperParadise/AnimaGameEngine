/*
#ifndef __CUBE_GO_H__
#define __CUBE_GO_H__

#include "GameObject.h"
#include <string>
#include "libraries/glew-2.0.0/include/GL/glew.h"

class ComponentMaterial;
class ComponentMesh;
class CubeGO : public GameObject
{
public:
	CubeGO(const std::string &name);
	~CubeGO();

private:

	GLfloat ambient[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat diffuse[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat specular[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat shininess = 32.0f;

	float *vertices = nullptr;
	float *normals = nullptr;
	float *tex_coord = nullptr;

	int num_vertices_coord = 108;
	int num_normals_coord = 108;
	int num_texture_coord = 72;
	int num_vertices = 36;

	void CreateVertexInfo();
};



#endif // !__CUBE_GO_H__

*/