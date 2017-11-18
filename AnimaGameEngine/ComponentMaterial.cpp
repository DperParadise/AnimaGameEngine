#include "ComponentMaterial.h"
#include "libraries/assimp/include/assimp/scene.h"
#include "TextureManager.h"

ComponentMaterial::ComponentMaterial(const std::string &name, 
	bool act, 
	GameObject *go, 
	aiMesh *mesh, 
	const aiScene *scene, 
	const char *file_name) : Component(name, act, go)
{
	Load(mesh, scene, file_name);
}

ComponentMaterial::ComponentMaterial(const std::string &name, 
	bool act, 
	GameObject *go, 
	float *ambient, 
	float *diffuse, 
	float *specular, 
	float shininess) : Component(name, act, go)
{
	memcpy(material.ambient, ambient, 4 * sizeof(float));
	memcpy(material.diffuse, diffuse, 4 * sizeof(float));
	memcpy(material.specular, specular, 4 * sizeof(float));
	material.shininess = shininess;

	diffuse_texture = aiString("default_diff_tex");
	TextureManager::GetInstance()->LoadDefaultTexture(diffuse_texture);

	mat_name = "default material";
}

ComponentMaterial::~ComponentMaterial() {}

void ComponentMaterial::Update(float dt) {}

void ComponentMaterial::Enable()
{
	active = true;
}

void ComponentMaterial::Disable()
{
	active = false;
}

void ComponentMaterial::Load(aiMesh *mesh, const aiScene *scene, const char *file_name)
{
	//Load Material
	unsigned mat_index = mesh->mMaterialIndex;
	aiMaterial *mat = scene->mMaterials[mat_index];

	aiColor3D amb;
	aiColor3D diff;
	aiColor3D spec;
	aiString material_name;

	if (mat->Get(AI_MATKEY_COLOR_AMBIENT, amb) == AI_SUCCESS)
	{
		material.ambient[0] = amb.r;
		material.ambient[1] = amb.g;
		material.ambient[2] = amb.b;
	}
	if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, diff) == AI_SUCCESS)
	{
		material.diffuse[0] = diff.r;
		material.diffuse[1] = diff.g;
		material.diffuse[2] = diff.b;
	}
	if (mat->Get(AI_MATKEY_COLOR_SPECULAR, spec) == AI_SUCCESS)
	{
		material.specular[0] = spec.r;
		material.specular[1] = spec.g;
		material.specular[2] = spec.b;
	}

	mat->Get(AI_MATKEY_SHININESS, material.shininess);

	if (mat->Get(AI_MATKEY_NAME, material_name) == AI_SUCCESS)
		mat_name = material_name.data;
	else
		mat_name = "(null)";

	//Load Texture
	if (scene->mMaterials[mat_index]->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE) == 0)
	{
		diffuse_texture = aiString("default_diff_tex");
		TextureManager::GetInstance()->LoadDefaultTexture(diffuse_texture);
		return;
	}

	aiString texture_file;
	std::string texture_path = std::string(file_name);
	std::size_t found = texture_path.find_last_of("/\\");
	char separator = texture_path[found];
	texture_path = texture_path.substr(0, found);
	texture_path.push_back(separator);

	scene->mMaterials[mat_index]->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &texture_file);
	TextureManager::GetInstance()->Load(aiString(texture_path.append(texture_file.data)));

	diffuse_texture = aiString(texture_path);
}

void ComponentMaterial::SetTexture(const char *file_name)
{
	TextureManager::GetInstance()->Load(aiString(file_name));
}


