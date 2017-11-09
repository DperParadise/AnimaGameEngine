#ifndef __MODEL_H__
#define __MODEL_H__

#include "libraries/assimp/include/assimp/Importer.hpp"
#include <string>

struct aiScene;
struct aiNode;
class GameObject;
class Model
{	

public:
	Model(std::string name, const char *file);
	~Model();

	//void Draw();
	void Clear();

	std::string model_name;
	GameObject *model_go = nullptr;
	
private:
	const aiScene *scene;
	Assimp::Importer importer;	
	void Load(std::string name, const char *file);
	void LoadHierarchy(aiNode *node, GameObject *go, const char *file);
};

#endif
