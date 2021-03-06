#ifndef RESOURCES_CLASSES_H
#define RESOURCES_CLASSES_H

#include "resources_platform.h"
#include "resources_references.h"

namespace resources
{
	struct AtlasImageInput
	{
		int width; 
		int height;
		graphics::TextureImage2d* texture;
	};
	
	class IAtlasPacker : public IBaseObject
	{
	public:
		virtual void startPack(int pageSize) = 0;
		virtual void add(const char* path, const AtlasImageInput &e) = 0;
		virtual void finishPack() = 0;
		virtual void loadFiles() = 0;
		virtual bool ready() = 0;
		virtual bool canPack(int w, int h) = 0;
	};

	struct CompressedTextureHeader
	{
		static int GetSize() { return 28; }
		ushort attributes;
		ushort Format;
		int CompressionPercent;
		int OriginWidth;
		int OriginHeight;
		int BorderX;
		int BorderY;
		int RawDataOffset;
	};
	
	class ResourceConstants
	{
	public:
		static const int TextureSizeLimit = 4 * 1024;
		static const int TextureLoadSizeLimit = 4*1024;
	};
}

#endif /*RESOURCES_CLASSES_H*/

