#include "spine_classes.h"
#include "spine/Atlas.h"
#include "spine/extension.h"

void _spAtlasPage_createTexture(spAtlasPage* self, const char* path)
{
	graphics::TextureImage2d* texture;
	if (resources::ContentManager::_replaceSeparator)
	{
		std::string replace = path;
		for (int i = 0; i < replace.size(); i++)
		{
			if (replace[i] == '/')
				replace[i] = '_';
		}
		texture = resources::ContentManager::Default.addLoadTexture(replace.c_str(), self->width, self->height, llge::Rgba);
		resources::ContentManager::Default._loadedImages[replace] = texture;
	}
	else
	{
		texture = resources::ContentManager::Default.addLoadTexture(path, self->width, self->height, llge::Rgba);
		resources::ContentManager::Default._loadedImages[path] = texture;
	}

	self->rendererObject = texture;
}

void _spAtlasPage_disposeTexture(spAtlasPage* self)
{
	graphics::TextureImage2d* texture = (graphics::TextureImage2d*)self->rendererObject;
	resources::ContentManager::Default.addDisposeTexture(texture);
	resources::TexturesMap::iterator find = resources::ContentManager::Default._loadedImages.end();
	for (resources::TexturesMap::iterator it = resources::ContentManager::Default._loadedImages.begin(); it != resources::ContentManager::Default._loadedImages.end(); it++)
	{
		if (it->second == texture)
			find = it;
	}
	if (find != resources::ContentManager::Default._loadedImages.end())
		resources::ContentManager::Default._loadedImages.erase(find);
}

char* _spUtil_readFile(const char* path, int* length)
{
	*length = 0;
	return 0;
}