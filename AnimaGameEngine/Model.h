#ifndef __MODEL_H__
#define __MODEL_H__

#include "libraries/assimp/include/assimp/postprocess.h"

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

	Model(const char *file, unsigned int load_flags);
	~Model();
	std::string model_name;
	GameObject *model_go = nullptr;

private:
	const aiScene *scene;
	Importer *importer = nullptr;
	void Load(const char *file, unsigned int flags);
	GameObject* LoadHierarchy(aiNode *node, GameObject *parent_go, const char *file);
};

#endif
