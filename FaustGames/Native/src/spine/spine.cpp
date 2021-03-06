#include "spine_classes.h"
#include "spine/Atlas.h"
#include "spine/extension.h"
#include "SpineSkeletonResource.h"

void _spAtlasPage_createTexture(spAtlasPage* self, const char* path)
{
	graphics::TextureImage2d* texture;
	spine::AtlasRenderObject* renderObject = static_cast<spine::AtlasRenderObject *>(self->atlas->rendererObject);
	if (renderObject->texture != nullptr)
	{
		auto image = resources::TexturesLoader::Default.loadPngTexture(renderObject->texture);
		texture = graphics::TexturesPool::GetImage();// new graphics::TextureImage2d(false, true);
		texture->create();
		texture->setData(image);
		self->rendererObject = texture;
		return;
	}
	

	int w = static_cast<int>(core::Math::trunc(self->width*renderObject->applyedCompression));
	int h = static_cast<int>(core::Math::trunc(self->height*renderObject->applyedCompression));
	
	if (resources::ContentProvider::ReplaceSeparator)
	{
		std::string replace = path;
		for (int i = 0; i < replace.size(); i++)
		{
			if (replace[i] == '/')
				replace[i] = '_';
		}
		auto image = renderObject->texturesManager->loadImage(replace.c_str(), renderObject->pagesFormat);
		texture = static_cast<graphics::TextureImage2d*>(image->getTextureImageInstance());
		//resources::ContentManager::Default._loadedImages[replace] = texture;
	}
	else 
	{
		auto image = renderObject->texturesManager->loadImage(path, renderObject->pagesFormat);
		texture = static_cast<graphics::TextureImage2d*>(image->getTextureImageInstance());
		//resources::ContentManager::Default._loadedImages[path] = texture;
	}

	self->rendererObject = texture;
}

void _spAtlasPage_disposeTexture(spAtlasPage* self)
{
	
	//graphics::TextureImage2d* texture = (graphics::TextureImage2d*)self->rendererObject;
	//resources::ContentManager::Default.addDisposeTexture(texture);



    /*
	resources::TexturesMap::iterator find = resources::ContentManager::Default._loadedImages.end();
	for (resources::TexturesMap::iterator it = resources::ContentManager::Default._loadedImages.begin(); it != resources::ContentManager::Default._loadedImages.end(); it++)
	{
		if (it->second == texture)
			find = it;
	}
	if (find != resources::ContentManager::Default._loadedImages.end())
		resources::ContentManager::Default._loadedImages.erase(find);
     */
}

char* _spUtil_readFile(const char* path, int* length)
{
	*length = 0;
	return 0;
}
