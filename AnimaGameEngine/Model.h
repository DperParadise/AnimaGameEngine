#ifndef __MODEL_H__
#define __MODEL_H__

#include "libraries/assimp/include/postprocess.h"
#include <string>

class Importer;
struct aiScene;
struct aiNode;
class GameObject;
class Model
{	

public:
	enum  load_flags {
		PRE_TRANSFORM_VERTICES = aiPostProcessSteps::aiProcess_PreTransformVertices,
		FLIP_UVs = aiPostProcessSteps::aiProcess_FlipUVs,
		TRIANGULATE = aiPostProcessSteps::aiProcess_Triangulate
	};

	Model(const std::string &filePath, unsigned int load_flags = 0);
	~Model();
	

private:
	const aiScene *scene;
	Importer *importer = nullptr;
	std::string modelName;
	GameObject *modelGO = nullptr;
	void Load(const std::string &filePath, unsigned int flags);
	GameObject* LoadHierarchy(aiNode *node, GameObject *parentGO, const std::string &filePath);
};

#endif
