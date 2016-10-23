#ifndef CONTENT_MANAGER_H
#define CONTENT_MANAGER_H

#include "resources_classes.h"

namespace resources
{
	typedef std::map<std::string, graphics::TextureImage2d *> TexturesMap;

	struct LoadImageEntry
	{
		std::string fileName;
		graphics::TextureImage2d *textureImage;
	};

	class AtlasEntryInput
	{
	public:
		graphics::Image2dData *image;
		graphics::TextureImage2d* texture;
	};

	class AtlasEntry
	{
	public:
	};

	class IAtlasOnlinePacker : public IBaseObject
	{
	public:
		virtual bool insert(const AtlasEntryInput &input, AtlasEntry &output) = 0;
		virtual void applyCurrentPage() = 0;
	};

	class ContentManager : public llge::IContentManager, public llge::ITextureBuffer2d
	{
	public:
		ContentManager();
		void cleanup();
		unsigned int registerTexture(const char *name);
		graphics::Image2dData * loadUnregisteredTexture(const char *name);
		graphics::Image2dData * loadUnregisteredPvrTexture(const char *name);
		graphics::Image2dData * loadUnregisteredEtcTexture(const char *name);
		char* loadString(const char *name);
		graphics::TextureImage2d * addLoadTexture(const char *name);
		void addDisposeTexture(graphics::TextureImage2d *image);
		graphics::Image2dData * loadTexture(int id);
		void open();
		void close();
		
		virtual llge::IContentAtlasMap * API_CALL getContentAtlasMap();
		virtual void API_CALL replaceSeparator(bool value);
		//virtual void API_CALL setObbFile(char * obbFile);
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
		static ContentManager Default;
		static bool _replaceSeparator;
		TexturesMap _loadedImages;
		void *getBuffer() const;
		int getBufferSize() const;
	private:
		bool tryPlaceIntoAtlas(graphics::Image2dData* image, graphics::TextureImage2d* texture);

		std::vector<std::string> _files;
		std::vector<LoadImageEntry> _loadEntries;
		std::vector<graphics::TextureImage2d *> _disposeEntries;
		graphics::Image2dData *_image;
		bool _isOpened;
		//IAtlasOnlinePacker* _packerRGBA;
		static bool ImageSizeLoaded;
		static int ImageBufferSize;
		static int ImageMaxHeight;
		static int ImageMaxWidth;
	};		
}

#endif /*CONTENT_MANAGER_H*/