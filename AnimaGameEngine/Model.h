#ifndef __MODEL_H__
#define __MODEL_H__

#include <string>
class Importer;
struct aiScene;
struct aiNode;
class GameObject;
class Model
{	

public:
	Model(std::string name, const char *file);
	~Model();

	std::string model_name;
	GameObject *model_go = nullptr;
	
private:
	const aiScene *scene;
	Importer *importer = nullptr;
	void Load(std::string name, const char *file);
	void LoadHierarchy(aiNode *node, GameObject *go, const char *file);
};

#endif
