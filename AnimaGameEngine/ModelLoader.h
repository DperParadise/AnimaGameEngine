#ifndef __MODEL_H__
#define __MODEL_H__

#include <assimp/postprocess.h>
#include <string>

struct aiScene;
struct aiNode;
class GameObject;
class Skeleton;
class aiMesh;
class Mesh;

class ModelLoader
{	

public:
	enum  load_flags {
		PRE_TRANSFORM_VERTICES = aiPostProcessSteps::aiProcess_PreTransformVertices,
		FLIP_UVs = aiPostProcessSteps::aiProcess_FlipUVs,
		TRIANGULATE = aiPostProcessSteps::aiProcess_Triangulate
	};

	ModelLoader();
	~ModelLoader();
	static GameObject* Load(const std::string &filePath, unsigned int flags);
	static GameObject* LoadMD5(const std::string &filePath, unsigned int flags, Skeleton **outSkeleton);

private:
	static  const aiScene *scene;
	static GameObject *modelGO;
	static GameObject* LoadHierarchy(const aiNode *node, GameObject *parentGO, const std::string &filePath);
	static GameObject* LoadModelMD5(const aiNode *node, const std::string &filePath, Skeleton **outSkeleton);
};

#endif
