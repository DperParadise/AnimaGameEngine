#include "ComponentMaterial.h"
#include "libraries/assimp/include/assimp/scene.h"
#include <string>
#include "TextureManager.h"

ComponentMaterial::ComponentMaterial(component_type t, bool act, GameObject *go, aiMesh *mesh, aiScene *scene, const char *file_name) : Component(t, act, go)
{	
	Load(mesh, scene, file_name);
}

ComponentMaterial::~ComponentMaterial(){}

void ComponentMaterial::Update() {}

void ComponentMaterial::Enable() 
{
	active = true;
}

void ComponentMaterial::Disable()
{
	active = false;
}

void ComponentMaterial::Load(aiMesh *mesh, aiScene *scene, const char *file_name)
{
	//Load Material
	unsigned mat_index = mesh->mMaterialIndex;
	aiMaterial *mat = scene->mMaterials[mat_index];

	mat->Get(AI_MATKEY_COLOR_AMBIENT, material.ambient);
	mat->Get(AI_MATKEY_COLOR_DIFFUSE, material.diffuse);
	mat->Get(AI_MATKEY_COLOR_SPECULAR, material.specular);
	mat->Get(AI_MATKEY_SHININESS, material.shininess);

	//Load Texture
	aiString texture_file;
	std::string texture_path = std::string(file_name);
	std::size_t found = texture_path.find_last_of("/\\");
	char separator = texture_path[found];
	texture_path = texture_path.substr(0, found);
	texture_path.push_back(separator);

	scene->mMaterials[mat_index]->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &texture_file);	
	TextureManager::GetInstance()->Load(aiString(texture_path.append(texture_file.data)));

	texture = aiString(texture_path);
}


