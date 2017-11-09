#include "Model.h"
#include "libraries/assimp/include/assimp/scene.h"
#include "libraries/assimp/include/assimp/postprocess.h"
#include "Globals.h"
#include "GameObject.h"
#include "ModuleScene.h"

Model::Model(std::string name, const char *file)
{
	Load(name, file);
}

Model::~Model() {}

void Model::Load(std::string name, const char *file)
{
	scene = importer.ReadFile(file,  /*aiPostProcessSteps::aiProcess_PreTransformVertices |*/ 
		aiPostProcessSteps::aiProcess_FlipUVs |
		aiPostProcessSteps::aiProcess_Triangulate /*| 
	aiProcessPreset_TargetRealtime_Fast*/);
	
	model_go = new GameObject(name);
	
	aiNode *root_node = scene->mRootNode;
	LoadHierarchy(root_node, model_go, file);
}

void Model::LoadHierarchy(aiNode *node, GameObject *go, const char *file)
{
	if (node->mNumChildren != 0)
	{
		for (uint i = 0; i < node->mNumChildren; i++)
		{
			GameObject *child_go = new GameObject(std::string("child_go"));
			go->children_go.push_back(child_go);

			aiNode *child_node = node->mChildren[i];

			LoadHierarchy(child_node, child_go, file);
		}
	}
	else
	{
		//load component transform
		go->CreateTransformComp(node);

		//load component mesh and material
		for (uint i = 0; i < node->mNumMeshes; i++)
		{
			uint mesh_index = node->mMeshes[i];
			go->CreateMeshComp(scene->mMeshes[mesh_index]);
			go->CreateMaterialComp(scene->mMeshes[mesh_index], scene, file);		
		}
	}
}

//void Model::Draw()
//{
//
//
//
//
//	aiString texture_file;
//	glEnableClientState(GL_VERTEX_ARRAY);
//	glEnableClientState(GL_NORMAL_ARRAY);
//	if (num_textures != 0)
//	{
//		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//	}
//
//	for (uint i = 0; i < scene->mNumMeshes; i++)
//	{	
//		uint mat_index = scene->mMeshes[i]->mMaterialIndex;
//		if (scene->mMaterials[mat_index]->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE) != 0)
//		{
//			scene->mMaterials[mat_index]->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &texture_file);
//			std::string texture_path(root_path);
//			unsigned int tex_id = textureManager->GetInstance()->Load(aiString(texture_path.append(texture_file.data)));
//			glBindTexture(GL_TEXTURE_2D, tex_id);
//			glTexCoordPointer(2, GL_FLOAT, 0, uv_array[i]);
//		}
//
//		glVertexPointer(3, GL_FLOAT, 0, vertex_array[i]);	
//		glNormalPointer(GL_FLOAT, 0, normal_array[i]);
//
//		glMaterialfv(GL_FRONT, GL_AMBIENT, materials[i].ambient);
//		glMaterialfv(GL_FRONT, GL_DIFFUSE, materials[i].diffuse);
//		glMaterialfv(GL_FRONT, GL_SPECULAR, materials[i].specular);
//		glMaterialf(GL_FRONT, GL_SHININESS, materials[i].shininess);
//
//		uint num_verts = scene->mMeshes[i]->mNumFaces * 3;
//		glDrawArrays(GL_TRIANGLES, 0, num_verts);	
//	}
//
//	glDisableClientState(GL_VERTEX_ARRAY);
//	glDisableClientState(GL_NORMAL_ARRAY);
//	if (num_textures != 0)
//	{
//		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//	}
//	glBindTexture(GL_TEXTURE_2D, 0);	
//}

void Model::Clear() {}
