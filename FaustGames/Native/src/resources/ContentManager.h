#ifndef CONTENT_MANAGER_H
#define CONTENT_MANAGER_H

#include "resources_classes.h"
#include "ContentProvider.h"

namespace resources
{
	typedef std::map<std::string, graphics::TextureImage2d *> TexturesMap;
	
	struct LoadImageEntry
	{
		std::string fileName;
		graphics::TextureImage2d *textureImage;
		int w;
		int h;
		llge::TextureQueryFormat queryFormat;
	};

	struct LoadRegisrtyEntry
	{
		std::string fileName;
	};

	struct ImageInfo
	{
		int Width;
		int Height;
		graphics::Image2dFormat::e Format;

		ImageInfo(int width, int height, graphics::Image2dFormat::e format)
		{
			Width = width;
			Height = height;
			Format = format;
		}
	};

	class ContentManager : public llge::IContentManager, public llge::ITextureBuffer2d
	{
	public:
		ContentManager();
		void cleanup();
		unsigned int registerTexture(const char *name);
		ImageInfo loadUnregisteredTextureSize(const char *name, llge::TextureQueryFormat queryFormat);
		void loadPngTexture(void* data, graphics::Image2dData* result);
		graphics::Image2dData * loadUnregisteredTexture(const char *name, llge::TextureQueryFormat queryFormat);
		ImageInfo loadUnregisteredCompressedTextureSize(const char *name);
		graphics::Image2dData * loadUnregisteredCompressedTexture(const char *name);

		char* loadString(const char *name);
		graphics::TextureImage2d * addLoadTexture(const char *name, llge::TextureQueryFormat format);
		graphics::TextureImage2d * addLoadTexture(const char *name, int w, int h, llge::TextureQueryFormat format);
		void addDisposeTexture(graphics::TextureImage2d *image);
		graphics::Image2dData * loadTexture(int id);
		void open();
		void close();
		
		virtual void API_CALL startAtlasBuild() OVERRIDE;
		virtual void API_CALL finishAtlasBuild() OVERRIDE;


		virtual llge::IContentProvider* API_CALL getContentProvider() override;
		virtual llge::IContentAtlasMap * API_CALL getContentAtlasMap()OVERRIDE;
		virtual void API_CALL useCompression(llge::TextureImage2dFormat format) override;

		virtual void API_CALL replaceSeparator(bool value)OVERRIDE;
		//virtual void API_CALL setObbFile(char * obbFile);
		virtual int API_CALL registerImage(char * name) OVERRIDE;
		virtual void API_CALL startLoad()OVERRIDE;
		virtual bool API_CALL update()OVERRIDE;
		virtual void API_CALL loadImage(int id, llge::ITextureImage2d *textureImage, int w, int h, llge::TextureQueryFormat queryFormat) OVERRIDE;
		virtual llge::ITextureBuffer2d *API_CALL loadBuffer(int id)OVERRIDE;
		virtual void API_CALL finishLoad()OVERRIDE;
		virtual void API_CALL dispose()OVERRIDE;

		virtual llge::TextureImage2dFormat API_CALL getFormat()OVERRIDE;
		virtual int API_CALL getWidth()OVERRIDE;
		virtual int API_CALL getHeight()OVERRIDE;
		virtual IntPtr API_CALL getPixels()OVERRIDE;
		static ContentManager Default;
		static bool _replaceSeparator;
		//TexturesMap _loadedImages;
		void *getBuffer() const;
		int getBufferSize() const;
		llge::TextureImage2dFormat getAtlasFormat(llge::TextureQueryFormat format);
		static int ImageBufferSize;
		static int ImageMaxHeight;
		static int ImageMaxWidth;
	private:
		llge::TextureImage2dFormat _platformCompressionAtlasFormat;
		IAtlasPacker* queryPacker(llge::TextureQueryFormat format);
		std::vector<LoadRegisrtyEntry> _files;
		std::vector<LoadImageEntry> _loadEntries;
		std::vector<graphics::TextureImage2d *> _disposeEntries;
		const char* _compressionExt;
		graphics::Image2dData *_image;
		bool _isOpened;
		bool _isAtlasBuilderStarted;
		IAtlasPacker* _packers[llge::TFEnumSize];
		static bool ImageSizeLoaded;
	};		

}

#endif /*CONTENT_MANAGER_H*/
