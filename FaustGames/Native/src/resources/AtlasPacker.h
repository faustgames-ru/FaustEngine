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
		static IAtlasPlacer* pvrtc14;
		static IAtlasPlacer* atc;
		static IAtlasPlacer* etc2;
		static IAtlasPlacer* switchPlacer(llge::TextureImage2dFormat format);
		virtual int getPageBufferSize(int pageSize) = 0;
		virtual graphics::Image2dBlocksOrder::e getPageBlocksOrder();
		virtual void SetupAlign(AlignInfo &alignInfo) {}
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
		template<typename TPixel>
		static void placeImageWithBorder(const PlaceArgs &e);
	};

	template <typename TPixel>
	void AbstractPlacer::placeImage(const PlaceArgs& e)
	{
		TPixel* srcRow = static_cast<TPixel*>(static_cast<void*>(e.imageData->Pixels+ e.imageData->RawDataOffset));
		TPixel* dst= static_cast<TPixel*>(static_cast<void*>(e.pageData->Pixels + e.pageData->RawDataOffset));

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

	template <typename TPixel>
	void AbstractPlacer::placeImageWithBorder(const PlaceArgs& e)
	{
		TPixel* srcRow = static_cast<TPixel*>(static_cast<void*>(e.imageData->Pixels + e.imageData->RawDataOffset));
		TPixel* dst = static_cast<TPixel*>(static_cast<void*>(e.pageData->Pixels + e.pageData->RawDataOffset));

		int dstStride = e.alignInfo.alignPageWidth(e.pageData->Width);
		int srcStride = e.rect.width;

		TPixel* dstRow = dst + e.rect.x + e.rect.y*dstStride;

		int h = e.rect.height;
		int w = e.rect.width;

		for (int y = 0; y < h; y++)
		{
			TPixel* dstPixel = dstRow;
			TPixel* srcPixel = srcRow;
			for (int x = 0; x < w; x++)
			{
				*dstPixel = *srcPixel;
				++srcPixel;
				++dstPixel;
			}
			dstRow += dstStride;
			srcRow += srcStride;
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

	class AtlasPlacerPvrtc14 : public IAtlasPlacer
	{
	public:
		virtual int getPageBufferSize(int pageSize) override;
		virtual void placeImage(const PlaceArgs &e) override;
		virtual void SetupAlign(AlignInfo &alignInfo) override;
	};

	class AtlasPlacerAtc : public IAtlasPlacer
	{
	public:
		virtual int getPageBufferSize(int pageSize) override;
		virtual void placeImage(const PlaceArgs &e) override;
		virtual void SetupAlign(AlignInfo &alignInfo) override;
	};

	class AtlasPlacerEtc2 : public IAtlasPlacer
	{
	public:
		virtual int getPageBufferSize(int pageSize) override;
		virtual void placeImage(const PlaceArgs &e) override;
		virtual void SetupAlign(AlignInfo &alignInfo) override;
	};
}

#endif /*ATLAS_PACKER_H*/
