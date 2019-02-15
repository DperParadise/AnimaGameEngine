#include "TextureManager.h"
#include <glew_2.0.0/GL/glew.h>
#include <devil/il.h>
#include <cassert>

std::unique_ptr<TextureManager> TextureManager::instance;

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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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

unsigned TextureManager::LoadDefaultTexture(const aiString& tex_name)
{
	GLubyte def_texture[1][1][4] = { (GLubyte)128 , (GLubyte)128 , (GLubyte)128 , (GLubyte)255 };
	
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, def_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	return textures[tex_name] = texture;
}

TextureManager* TextureManager::GetInstance()
{
	if (!instance.get())
		instance.reset(new TextureManager);

	return instance.get();
}
