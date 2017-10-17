#ifndef __TEXTURE_MANAGER_H__
#define __TEXTURE_MANAGER_H__

#include <cstring>
#include <map>
#include "libraries/assimp/include/assimp/postprocess.h"
#include "libraries\DevIL_Windows_SDK\include\IL\il.h"

class TextureManager
{
	struct LessString
	{
		bool operator()(const aiString& left, const aiString& right) const
		{
			return strcmp(left.data, right.data) < 0;
		}
	};

	typedef std::map<aiString, unsigned, LessString> textureList;

	textureList textures;
	static TextureManager *instance;
	ILuint image;
	ILubyte *image_data = nullptr;


public:
	TextureManager();
	~TextureManager();

	unsigned Load(const aiString& file);
	void Clear();

	static TextureManager *GetInstance();
};



#endif
