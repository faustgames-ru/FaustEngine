#ifndef ATLAS_PACKER_H
#define ATLAS_PACKER_H

#include "llge.h"
#include "resources_classes.h"
#include "../../src_rectanglebinpack/MaxRectsBinPack.h"

namespace resources
{
	struct AtlasImageEntry
	{
		std::string path;
		AtlasImageInput input;
		AtlasImageEntry(const char* p, const AtlasImageInput& e);
	};

	class AtlasRect
	{
	public:
		rbp::Rect rect;
		AtlasImageEntry* entry;
		AtlasRect(rbp::Rect rect);
	};

	class AtlasPage
	{
	public:
		std::vector<AtlasRect> rects;
	};
	
	class AtlasTexturesPool
	{
	public:
		static AtlasTexturesPool Default;
		graphics::TextureAtlasPage* queryPage();
		void clear();
	private:
		std::vector<graphics::TextureAtlasPage* > _pages;
		int _index;
	};

	class AtlasPacker : public IAtlasPacker
	{
	public:
		static AtlasPacker* create(llge::TextureImage2dFormat format);
		virtual void startPack(int pageSize) OVERRIDE;
		virtual void add(const char* path, const AtlasImageInput &e) OVERRIDE;
		virtual void finishPack() OVERRIDE;
		virtual void loadFiles() OVERRIDE;
		virtual void createPageData() = 0;
		virtual void placeImage(AtlasRect rect, graphics::Image2dData *imageData) = 0;
		virtual bool ready() OVERRIDE;
	protected:
		explicit AtlasPacker(llge::TextureImage2dFormat format);
		int sizeAlign;
		int sizeBorders;
		int pixelBorders;
		int _pageSize;
		graphics::Image2dData* _pageData;
		llge::TextureImage2dFormat _internalFormat;
	private:
		float getAlignedSizeWithBorder(float size);
		float getAlignedSize(float size);
		float scaleX(int x);
		float scaleY(int y);
		
		void releasePages();
		void releaseEntries();
		std::vector<AtlasImageEntry *> _inputPack;
		std::vector<AtlasPage* > _pages;
		bool _ready;
	};

	class AtlasPackerRgba : public AtlasPacker
	{
	public:
		explicit AtlasPackerRgba(llge::TextureImage2dFormat format);
		virtual void createPageData() OVERRIDE;
		virtual void placeImage(AtlasRect rect, graphics::Image2dData *image) OVERRIDE;
		void placeRgb(AtlasRect rect, graphics::Image2dData *imageData);
		void placeRgbRow(AtlasRect rect, int ySrc, int yDst, graphics::Image2dData* imageData);
		void placeRgbCol(AtlasRect rect, int xSrc, int xDst, graphics::Image2dData* image);
		void placeRgba(AtlasRect rect, graphics::Image2dData *imageData);
		void placeRgbaRow(AtlasRect rect, int ySrc, int yDst, graphics::Image2dData* imageData);
		void placeRgbaCol(AtlasRect rect, int xSrc, int xDst, graphics::Image2dData* image);
	};

	class AtlasPackerRgb : public AtlasPacker
	{
	public:
		explicit AtlasPackerRgb(llge::TextureImage2dFormat format);
		virtual void createPageData() OVERRIDE;
		virtual void placeImage(AtlasRect rect, graphics::Image2dData *image) OVERRIDE;

		void placeRgb(AtlasRect rect, graphics::Image2dData *imageData);
		void placeRgbRow(AtlasRect rect, int ySrc, int yDst, graphics::Image2dData* imageData);
		void placeRgbCol(AtlasRect rect, int xSrc, int xDst, graphics::Image2dData* image);
	};

	class AtlasPackerPvr14 : public AtlasPacker
	{
	public:
		explicit AtlasPackerPvr14(llge::TextureImage2dFormat format);
		virtual void createPageData() OVERRIDE;
		virtual void placeImage(AtlasRect rect, graphics::Image2dData *image) OVERRIDE;

		void place(AtlasRect rect, graphics::Image2dData *imageData);
		void placeRow(AtlasRect rect, int ySrc, int yDst, graphics::Image2dData* imageData);
	};
}

#endif /*ATLAS_PACKER_H*/
