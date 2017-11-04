#include "ComponentTexture.h"
#include "TextureManager.h"
#include "Importer.h"


ComponentTexture::ComponentTexture(const char *file, component_type t, bool act, GameObject *go) : Component(t, act, go)
{
	if (file == nullptr)
	{
		MYLOG("Error invalid nullptr file: %s");
		return;
	}

	LoadTextures(file);
}

ComponentTexture::~ComponentTexture()
{
	texture_paths.clear();

	if (num_textures != 0)
	{
		for (uint i = 0; i < num_meshes; i++)
		{
			RELEASE_ARRAY(uv_array[i]);
		}

		RELEASE_ARRAY(uv_array);
	}

}

void ComponentTexture::Update() {}

void ComponentTexture::Enable()
{
	active = true;
}

void ComponentTexture::Disable()
{
	active = false;
}

void ComponentTexture::LoadTextures(const char *file)
{
	Importer *importer = nullptr;
	
	const aiScene *scene = importer->GetInstance()->ReadFile(file, /*aiPostProcessSteps::aiProcess_PreTransformVertices |*/
		aiPostProcessSteps::aiProcess_FlipUVs | aiPostProcessSteps::aiProcess_Triangulate /*|aiProcessPreset_TargetRealtime_Fast*/);

	if (scene == NULL)
	{
		MYLOG("Error importing scene: %s", importer->GetInstance()->GetErrorString());
		return;
	}

	if (scene->mNumMeshes == 0)
	{
		MYLOG("No meshes in the imported scene");
		return;
	}
	 num_meshes = scene->mNumMeshes;
	
	aiString texture_file;
	std::string root_path = std::string(file);
	std::size_t found = root_path.find_last_of("/\\");
	char separator = root_path[found];
	root_path = root_path.substr(0, found);
	root_path.push_back(separator);

	for (uint i = 0; i < num_meshes; i++)
	{
		uint mat_index = scene->mMeshes[i]->mMaterialIndex;
		aiMaterial *mat = scene->mMaterials[mat_index];
		if (mat->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE) != 0)
		{
			num_textures++;
			scene->mMaterials[mat_index]->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &texture_file);
			std::string texture_path(root_path);
			TextureManager::GetInstance()->Load(aiString(texture_path.append(texture_file.data)));
			texture_paths.push_back(texture_path);
		}
		else
		{
			texture_paths.push_back(std::string(""));
		}
	}

	if (num_textures == 0)
		return;

	uv_array = new float*[num_textures];
	for (uint i = 0; i < num_textures; i++)
	{
		uv_array[i] = nullptr;
	}
	
	for (uint i = 0; i < num_meshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[i];		
		
		if (!mesh->HasTextureCoords(0))
			continue;
			
		uv_array[i] = new float[mesh->mNumFaces * 3 * 2];

		for (uint j = 0; j < mesh->mNumFaces; j++)
		{
			const aiFace& face = mesh->mFaces[j];

			for (uint k = 0; k<3; k++)
			{				
				aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[k]];
				memcpy(uv_array[i], &uv, sizeof(float) * 2);
				uv_array[i] += 2;					
			}
		}	
			uv_array[i] -= mesh->mNumFaces * 3 * 2;	
	}
}

bool ComponentTexture::HasTexture() const
{
	return (bool)num_textures;
}

