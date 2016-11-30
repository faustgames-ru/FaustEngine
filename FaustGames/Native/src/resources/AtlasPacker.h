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

	struct AlignInfo
	{
		AlignInfo();
		int blockSizeX;
		int blockSizeY;
		int borderBlockCount;
		int alignWidth(int w) const;
		int alignHeight(int h) const;
		int alignPageWidth(int w) const;
		int alignPageHeight(int h) const;
	};

	struct PlaceArgs
	{
		AlignInfo alignInfo;
		rbp::Rect rect;
		graphics::Image2dData* pageData;
		graphics::Image2dData *imageData;
	};
	
	class IAtlasPlacer : public IBaseObject
	{
	public:
		static IAtlasPlacer* rgba8888;
		static IAtlasPlacer* rgba4444;
		static IAtlasPlacer* rgb888;
		static IAtlasPlacer* pvrtc12;
		static IAtlasPlacer* pvrtc14;
		static IAtlasPlacer* switchPlacer(llge::TextureImage2dFormat format);
		virtual int getPageBufferSize(int pageSize) = 0;
		virtual void placeImage(const PlaceArgs &e) = 0;
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
		explicit AtlasPacker(llge::TextureImage2dFormat format, IAtlasPlacer* placer);
		virtual void startPack(int size) OVERRIDE;
		virtual void add(const char* path, const AtlasImageInput &e) OVERRIDE;
		virtual void finishPack() OVERRIDE;
		virtual void loadFiles() OVERRIDE;

		virtual bool ready() OVERRIDE;

		AtlasImageInput alignInput(const AtlasImageInput &e);
		llge::TextureQueryFormat getQueryFormat();
		graphics::Image2dFormat::e getFormat();
	protected:
		AlignInfo alignInfo;
		int pageSize;
		graphics::Image2dData* _pageData;
		llge::TextureImage2dFormat _internalFormat;
		IAtlasPlacer* placer;
	private:
		
		void releasePages();
		void releaseEntries();
		std::vector<AtlasImageEntry *> _inputPack;
		std::vector<AtlasPage* > _pages;
		bool _ready;
	};



	class AbstractPlacer
	{
	public:
		template<typename TPixel>
		static void placeImage(const PlaceArgs &e);
	};

	template <typename TPixel>
	void AbstractPlacer::placeImage(const PlaceArgs& e)
	{
		TPixel* srcRow = static_cast<TPixel*>(static_cast<void*>(e.imageData->Pixels));
		TPixel* dst= static_cast<TPixel*>(static_cast<void*>(e.pageData->Pixels));

		int dstStride = e.alignInfo.alignPageWidth(e.pageData->Width);
		int srcStride = e.alignInfo.alignPageWidth(e.imageData->Width);

		TPixel* dstRow = dst + e.rect.x + e.rect.y*dstStride;

		int h = e.rect.height - e.alignInfo.borderBlockCount * 2;
		int w = e.rect.width - e.alignInfo.borderBlockCount * 2;
		for (int y = 0; y < e.alignInfo.borderBlockCount; y++)
		{
			TPixel* dstPixel = dstRow;
			TPixel* srcPixel = srcRow;
			for (int x = 0; x < e.alignInfo.borderBlockCount; x++)
			{
				*dstPixel = *srcPixel;
				++dstPixel;
			}
			for (int x = 0; x < w; x++)
			{
				*dstPixel = *srcPixel;
				++srcPixel;
				++dstPixel;
			}
			--srcPixel;
			for (int x = 0; x < e.alignInfo.borderBlockCount; x++)
			{
				*dstPixel = *srcPixel;
				++dstPixel;
			}
			dstRow += dstStride;
		}
		for (int y = 0; y < h; y++)
		{
			TPixel* dstPixel = dstRow;
			TPixel* srcPixel = srcRow;
			for (int x = 0; x < e.alignInfo.borderBlockCount; x++)
			{
				*dstPixel = *srcPixel;
				++dstPixel;
			}
			for (int x = 0; x < w; x++)
			{
				*dstPixel = *srcPixel;
				++srcPixel;
				++dstPixel;
			}
			--srcPixel;
			for (int x = 0; x < e.alignInfo.borderBlockCount; x++)
			{
				*dstPixel = *srcPixel;
				++dstPixel;
			}
			dstRow += dstStride;
			srcRow += srcStride;
		}
		srcRow -= srcStride;
		for (int y = 0; y < e.alignInfo.borderBlockCount; y++)
		{
			TPixel* dstPixel = dstRow;
			TPixel* srcPixel = srcRow;
			for (int x = 0; x < e.alignInfo.borderBlockCount; x++)
			{
				*dstPixel = *srcPixel;
				++dstPixel;
			}
			for (int x = 0; x < w; x++)
			{
				*dstPixel = *srcPixel;
				++srcPixel;
				++dstPixel;
			}
			--srcPixel;
			for (int x = 0; x < e.alignInfo.borderBlockCount; x++)
			{
				*dstPixel = *srcPixel;
				++dstPixel;
			}
			dstRow += dstStride;
		}
	}

	class AtlasPlacerRgba : public IAtlasPlacer
	{
	public:
		virtual int getPageBufferSize(int pageSize) override;
		virtual void placeImage(const PlaceArgs &e) override;
	};

	class AtlasPlacerRgba4444 : public IAtlasPlacer
	{
	public:
		virtual int getPageBufferSize(int pageSize) override;
		virtual void placeImage(const PlaceArgs &e) override;
	};

	class AtlasPlacerRgb : public IAtlasPlacer
	{
	public:
		virtual int getPageBufferSize(int pageSize) override;
		virtual void placeImage(const PlaceArgs &e) override;
	};

	class AtlasPlacerPvrtc12 : public IAtlasPlacer
	{
	public:
		virtual int getPageBufferSize(int pageSize) override;
		virtual void placeImage(const PlaceArgs &e) override;
	};

	class AtlasPlacerPvrtc14 : public IAtlasPlacer
	{
	public:
		virtual int getPageBufferSize(int pageSize) override;
		virtual void placeImage(const PlaceArgs &e) override;
	};
	/*
	class AtlasPackerRgba : public AtlasPacker
	{
	public:
		explicit AtlasPackerRgba(llge::TextureImage2dFormat format);
		//virtual void createPageData() OVERRIDE;
		//virtual void clearPageData() OVERRIDE;
		//virtual void placeImage(AtlasRect rect, graphics::Image2dData *image) OVERRIDE;
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
		//virtual void createPageData() OVERRIDE;
		//virtual void clearPageData() OVERRIDE;
		//virtual void placeImage(AtlasRect rect, graphics::Image2dData *image) OVERRIDE;

		void placeRgb(AtlasRect rect, graphics::Image2dData *imageData);
		void placeRgbRow(AtlasRect rect, int ySrc, int yDst, graphics::Image2dData* imageData);
		void placeRgbCol(AtlasRect rect, int xSrc, int xDst, graphics::Image2dData* image);
	};

	class AtlasPackerPvr14 : public AtlasPacker
	{
	public:
		explicit AtlasPackerPvr14(llge::TextureImage2dFormat format);
		//virtual void createPageData() OVERRIDE;
		//virtual void clearPageData() OVERRIDE;
		//virtual void placeImage(AtlasRect rect, graphics::Image2dData *image) OVERRIDE;

		void place(AtlasRect rect, graphics::Image2dData *imageData);
		void placeRow(AtlasRect rect, int ySrc, int yDst, graphics::Image2dData* imageData);
	};
	*/
}

#endif /*ATLAS_PACKER_H*/
