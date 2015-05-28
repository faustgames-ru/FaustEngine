#include "spine_classes.h"
#include "spine\Atlas.h"
#include "spine\extension.h"

void _spAtlasPage_createTexture(spAtlasPage* self, const char* path)
{
	// todo: resolve multy registration
	
	graphics::TextureImage2d* texture = resources::ContentManager::Default.addLoadTexture(path);

	// todo: detect size;

	//self->width = imageData->Width;
	//self->height = imageData->Height;

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