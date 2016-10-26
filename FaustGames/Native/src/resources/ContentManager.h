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

	struct LoadRegisrtyEntry
	{
		std::string fileName;
		int w;
		int h;
		llge::TextureImage2dFormat format;
	};
	/*
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
	*/
	class ContentManager : public llge::IContentManager, public llge::ITextureBuffer2d
	{
	public:
		ContentManager();
		void cleanup();
		unsigned int registerTexture(const char *name, int w, int h, llge::TextureImage2dFormat format);
		graphics::Image2dData * loadUnregisteredTexture(const char *name);
		graphics::Image2dData * loadUnregisteredPvrTexture(const char *name);
		graphics::Image2dData * loadUnregisteredEtcTexture(const char *name);
		char* loadString(const char *name);
		graphics::TextureImage2d * addLoadTexture(const char *name);
		graphics::TextureImage2d * addLoadTexture(const char *name, int w, int h, llge::TextureImage2dFormat format);
		void addDisposeTexture(graphics::TextureImage2d *image);
		graphics::Image2dData * loadTexture(int id);
		void open();
		void close();
		
		virtual void API_CALL startAtlasBuild() OVERRIDE;
		virtual void API_CALL finishAtlasBuild() OVERRIDE;


		virtual llge::IContentAtlasMap * API_CALL getContentAtlasMap()OVERRIDE;
		virtual void API_CALL replaceSeparator(bool value)OVERRIDE;
		//virtual void API_CALL setObbFile(char * obbFile);
		virtual int API_CALL registerImage(char * name, int w, int h, llge::TextureImage2dFormat format)OVERRIDE;
		virtual void API_CALL startLoad()OVERRIDE;
		virtual bool API_CALL update()OVERRIDE;
		virtual void API_CALL loadImage(int id, llge::ITextureImage2d *textureImage) OVERRIDE;
		virtual llge::ITextureBuffer2d *API_CALL loadBuffer(int id)OVERRIDE;
		virtual void API_CALL finishLoad()OVERRIDE;
		virtual void API_CALL dispose()OVERRIDE;

		virtual llge::TextureImage2dFormat API_CALL getFormat()OVERRIDE;
		virtual int API_CALL getWidth()OVERRIDE;
		virtual int API_CALL getHeight()OVERRIDE;
		virtual IntPtr API_CALL getPixels()OVERRIDE;
		static ContentManager Default;
		static bool _replaceSeparator;
		TexturesMap _loadedImages;
		void *getBuffer() const;
		int getBufferSize() const;

		static int ImageBufferSize;
		static int ImageMaxHeight;
		static int ImageMaxWidth;
	private:
		IAtlasPacker* queryPacker(llge::TextureImage2dFormat format);
		std::vector<LoadRegisrtyEntry> _files;
		std::vector<LoadImageEntry> _loadEntries;
		std::vector<graphics::TextureImage2d *> _disposeEntries;
		graphics::Image2dData *_image;
		bool _isOpened;
		bool _isAtlasBuilderStarted;
		IAtlasPacker* _packers[llge::TextureFormatEnumSize];
		static bool ImageSizeLoaded;
	};		
}

#endif /*CONTENT_MANAGER_H*/