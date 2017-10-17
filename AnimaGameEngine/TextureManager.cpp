#include "TextureManager.h"

TextureManager::TextureManager(){}
TextureManager::~TextureManager(){}

unsigned TextureManager::Load(const aiString& file)
{
	

	

}

void TextureManager::Clear()
{

}

TextureManager *TextureManager::instance = nullptr;

TextureManager* TextureManager::GetInstance()
{
	if (instance == nullptr)
		instance = new TextureManager();

	return instance;
}