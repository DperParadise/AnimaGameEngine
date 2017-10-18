#include "TextureManager.h"
#include "libraries/glew-2.0.0/include/GL/glew.h"
#include "libraries/DevIL_Windows_SDK/include/IL/il.h"
#include <cassert>

std::auto_ptr<TextureManager> TextureManager::instance;

TextureManager::TextureManager(){}

TextureManager::~TextureManager()
{
	Clear();
}


unsigned TextureManager::Load(const aiString& file)
{
	TextureList::iterator it = textures.find(file);

	if (it != textures.end())
	{
		return it->second;
	}

	return textures[file] = ForceLoad(file);
}

void TextureManager::Clear()
{
	for (TextureList::iterator it = textures.begin(); it != textures.end(); ++it)
	{
		glDeleteTextures(1, &it->second);
	}

	textures.clear();
}


unsigned TextureManager::ForceLoad(const aiString& file)
{
	ILuint imageId;
	ilGenImages(1, &imageId);
	ilBindImage(imageId);

	if (ilLoadImage(file.data))
	{
		GLuint textureId = 0;
		glGenTextures(1, &textureId);

		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		int components = 3;
		int format = GL_RGB;

		switch (ilGetInteger(IL_IMAGE_FORMAT))
		{
		case IL_RGB:
			components = 3;
			format = GL_RGB;
			break;

		case IL_RGBA:
			components = 4;
			format = GL_RGBA;
			break;

		case IL_BGR:
			components = 3;
			format = GL_BGR_EXT;
			break;

		case IL_BGRA:
			components = 4;
			format = GL_BGRA_EXT;
			break;

		default:
			assert(false);
		}

		ILubyte* data = ilGetData();
		int width = ilGetInteger(IL_IMAGE_WIDTH);
		int height = ilGetInteger(IL_IMAGE_HEIGHT);

		glTexImage2D(GL_TEXTURE_2D, 0, components, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		ilDeleteImages(1, &imageId);

		glBindTexture(GL_TEXTURE_2D, 0);

		return textureId;
	}

	return 0;
}

TextureManager* TextureManager::GetInstance()
{
	if (!instance.get())
		instance.reset(new TextureManager);

	return instance.get();
}
