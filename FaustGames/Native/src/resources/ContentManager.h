#ifndef CONTENT_MANAGER_H
#define CONTENT_MANAGER_H

#include "resources_classes.h"

namespace resources
{
	struct LoadImageEntry
	{
		std::string fileName;
		graphics::TextureImage2d *textureImage;
	};

	struct RectRegion
	{
		int X;
		int Y;
		int Width;
		int Height;
	};

	struct ContentAtlasRect
	{
		int PageIndex;
		RectRegion Region;
	};

	struct ContentAtlasPage
	{
		ContentAtlasPage() :Width(0), Height(0) {}
		int Width;
		int Height;
	};

	typedef std::map<std::string, ContentAtlasRect> ContentAtlasRects;

	class ContentAtlasMap : public llge::IContentAtlasMap
	{
	public:
		virtual void API_CALL resetMap();
		virtual void API_CALL addRect(char* name, int pageIndex, int x, int y, int width, int height);
		virtual void API_CALL loadTextures();
		bool loadImage(const char *name, llge::ITextureImage2d *textureImage);
	private:
		std::vector<ContentAtlasPage> _pages;
		ContentAtlasRects _rects;
		std::vector<graphics::TextureImage2d *> _textures;
	};

	class ContentManager : public llge::IContentManager, public llge::ITextureBuffer2d
	{
	public:
		ContentManager();
		void cleanup();
		unsigned int registerTexture(const char *name);
		graphics::Image2dData * loadUnregisteredTexture(const char *name);
		graphics::TextureImage2d * addLoadTexture(const char *name);
		void addDisposeTexture(graphics::TextureImage2d *image);
		graphics::Image2dData * loadTexture(int id);
		void open();
		void close();
		
		virtual llge::IContentAtlasMap * API_CALL getContentAtlasMap();
		virtual void API_CALL replaceSeparator(bool value);
		virtual void API_CALL setObbFile(char * obbFile);
		virtual int API_CALL registerImage(char * name);
		virtual void API_CALL startLoad();
		virtual bool API_CALL update();
		virtual void API_CALL loadImage(int id, llge::ITextureImage2d *textureImage);
		virtual llge::ITextureBuffer2d *API_CALL loadBuffer(int id);
		virtual void API_CALL finishLoad();
		virtual void API_CALL dispose();

		virtual llge::TextureImage2dFormat API_CALL getFormat();
		virtual int API_CALL getWidth();
		virtual int API_CALL getHeight();
		virtual IntPtr API_CALL getPixels();
		void useLoadRegion(bool value);
		void setLoadRegion(RectRegion loadRegion);
		void setPage(int pageWidth, int pageHeight);
		static ContentManager Default;
		static bool _replaceSeparator;
	private:
		std::vector<std::string> _files;
		std::vector<LoadImageEntry> _loadEntries;
		std::vector<graphics::TextureImage2d *> _disposeEntries;
		graphics::Image2dData *_image;
		bool _isOpened;
		RectRegion _loadRegion;
		bool _useLoadRegion;
		int _pageWidth;
		int _pageHeight;
		ContentAtlasMap AtlasMap;
#ifdef __ANDROID__
		static const int ImageBufferSize = 2048 * 2048; //(2048x2048x32bpp)
		static const int ImageMaxHeight = 2048;
		static const int ImageMaxWidth = 2048;
#else
		static const int ImageBufferSize = 4*2048 * 2048; //(4096x4096x32bpp)
		static const int ImageMaxHeight = 2*2048;
		static const int ImageMaxWidth = 2*2048;
#endif
	};	
}

#endif /*CONTENT_MANAGER_H*/