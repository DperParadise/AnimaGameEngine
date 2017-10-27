#include "ComponentMaterial.h"
#include "Globals.h"
#include "libraries/assimp/include/assimp/postprocess.h"
#include "Importer.h"

ComponentMaterial::ComponentMaterial(component_type t, bool act, GameObject *go, const char *file) : Component(t, act, go)
{
	if (file == nullptr)
	{
		Material default_mat;
		materials.push_back(default_mat);
	}
	else
	{
		LoadMaterials(file);
	}
}

ComponentMaterial::~ComponentMaterial() 
{
	materials.clear();
}

void ComponentMaterial::Update() {}

void ComponentMaterial::Enable() 
{
	active = true;
}

void ComponentMaterial::Disable()
{
	active = false;
}

void ComponentMaterial::LoadMaterials(const char * file)
{
	scene = importer->GetInstance()->ReadFile(file,  aiPostProcessSteps::aiProcess_PreTransformVertices |
		aiPostProcessSteps::aiProcess_FlipUVs | aiPostProcessSteps::aiProcess_Triangulate /*|aiProcessPreset_TargetRealtime_Fast*/);

	if (scene == NULL)
	{
		MYLOG("Error importing scene: %s", importer->GetInstance()->GetErrorString());
		return;
	}

	if (scene->mNumMeshes == 0)
	{
		MYLOG("No meshes in the imported object");
		return;
	}

	int mat_count = 0;
	for (uint i = 0; i < scene->mNumMeshes; i++)
	{
		//load materials 	
		Material material;
		aiColor3D ambient;
		aiColor3D diffuse;
		aiColor3D specular;
		float shininess;

		uint mat_index = scene->mMeshes[i]->mMaterialIndex;
		aiMaterial *mat = scene->mMaterials[mat_index];
		if (mat->Get(AI_MATKEY_COLOR_AMBIENT, ambient) == AI_SUCCESS)
		{
			material.ambient[0] = ambient.r;
			material.ambient[1] = ambient.g;
			material.ambient[2] = ambient.b;
		}		
		if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse) == AI_SUCCESS)
		{
			material.diffuse[0] = diffuse.r;
			material.diffuse[1] = diffuse.g;
			material.diffuse[2] = diffuse.b;
		}
		if (mat->Get(AI_MATKEY_COLOR_SPECULAR, specular) == AI_SUCCESS)
		{
			material.specular[0] = specular.r;
			material.specular[1] = specular.g;
			material.specular[2] = specular.b;
		}
		if (mat->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
		{
			material.shininess = shininess;
		}

		materials.push_back(material);
		
		/*MYLOG("%d  amb = (%f, %f, %f)   diff = (%f,%f,%f)   spec = (%f,%f,%f)   shin = %f", mat_count, material.ambient[0], material.ambient[1], material.ambient[2],
			material.diffuse[0], material.diffuse[1], material.diffuse[2], 
			material.specular[0], material.specular[1], material.specular[2], 
			material.shininess);*/

		mat_count++;
	}
	MYLOG("Loaded %d materials", mat_count);
}


