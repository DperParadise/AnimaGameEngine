#ifndef __TEXTURE_MANAGER_H__
#define __TEXTURE_MANAGER_H__

#include <cstring>
#include <map>
#include <memory>
#include "libraries/assimp/include/assimp/postprocess.h"

class TextureManager
{
	struct LessString
	{
		bool operator()(const aiString& left, const aiString& right) const
		{
			return strcmp(left.data, right.data) < 0;
		}
	};

	typedef std::map<aiString, unsigned, LessString> TextureList;

	TextureList textures;
	static std::unique_ptr<TextureManager> instance;
	

public:
	TextureManager();
	~TextureManager();

	unsigned Load(const aiString& file);
	unsigned TextureManager::ForceLoad(const aiString& file);
	unsigned LoadDefaultTexture(const aiString& tex_name);
	void Clear();

	static TextureManager *GetInstance();
};



#endif
