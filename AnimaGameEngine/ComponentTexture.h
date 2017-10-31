#ifndef __COMPONENT_TEXTURE_H__
#define __COMPONENT_TEXTURE_H__

#include "Component.h"
#include "libraries/assimp/include/assimp/scene.h"
#include <vector>
#include <string>
#include "Globals.h"

class GameObject;
class TextureManager;
class ComponentTexture : public Component
{
public:
	ComponentTexture(const char *file, component_type t, bool act, GameObject *go);
	~ComponentTexture();

	void Update();
	void Enable();
	void Disable();

	std::vector<std::string> texture_paths;
	TextureManager *texture_manager = nullptr;
	float **uv_array = nullptr;

private:

	uint num_meshes = 0;
	uint num_textures = 0;

	void LoadTextures(const char *file);
	
};


#endif
