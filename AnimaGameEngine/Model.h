#ifndef __MODEL_H__
#define __MODEL_H__

class Importer;
struct aiScene;
struct aiNode;
class GameObject;
class Model
{	

public:
	Model(const char *file);
	~Model();

	std::string model_name;
	GameObject *model_go = nullptr;
	
private:
	const aiScene *scene;
	Importer *importer = nullptr;
	void Load(const char *file);
	GameObject* LoadHierarchy(aiNode *node, GameObject *parent_go, const char *file);
};

#endif
