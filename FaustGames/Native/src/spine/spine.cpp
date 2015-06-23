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
		texture = resources::ContentManager::Default.addLoadTexture(replace.c_str());
	}
	else
	{
		texture = resources::ContentManager::Default.addLoadTexture(path);
	}
	self->rendererObject = texture;
}

void _spAtlasPage_disposeTexture(spAtlasPage* self)
{
	graphics::TextureImage2d* texture = (graphics::TextureImage2d*)self->rendererObject;
	resources::ContentManager::Default.addDisposeTexture(texture);
}

char* _spUtil_readFile(const char* path, int* length)
{
	*length = 0;
	return 0;
}