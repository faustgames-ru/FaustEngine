#ifndef TEXTURES_LOADER_H
#define TEXTURES_LOADER_H

#include "resources_classes.h"
#include "ContentProvider.h"

namespace resources
{

	struct ImageInfo
	{
		int Width;
		int Height;
		graphics::Image2dFormat::e Format;

		CompressedTextureHeader compressionHeader;

		ImageInfo()
		{
			Width = 0;
			Height = 0;
			Format = graphics::Image2dFormat::Rgba;
		}

		ImageInfo(int width, int height, graphics::Image2dFormat::e format)
		{
			Width = width;
			Height = height;
			Format = format;
		}
	};

	class ReadService
	{
	public:
		virtual ~ReadService();
		virtual void read(void *data, int size) = 0;
	};

	class ReadMemoryService: public ReadService
	{
	private:
		int _pos;
		void* _ptr;
	public:
		ReadMemoryService();
		void setup(void *data);
		virtual void read(void *data, int size) override;
	};

	class ReadFileService : public ReadService
	{
	public:
		virtual void read(void *data, int size) ;
	};

	class TexturesLoader
	{
	public:
		static TexturesLoader Default;
		static ReadService* ReadService;
		static ReadMemoryService ReadMemoryService;
		static ReadFileService ReadFileService;

		TexturesLoader();
		void useCompression(llge::TextureImage2dFormat format);
		ImageInfo loadTextureSize(const char *name, llge::TextureQueryFormat queryFormat);
		graphics::Image2dResourceData readImage(const char *name, llge::TextureQueryFormat queryFormat);
		graphics::Image2dResourceData emptyImage();
		static ImageInfo emptyImageInfo();

		void clearLoadingBuffer();
		void resizeLoadingBuffer(int size);

		graphics::Image2dResourceData loadPngTexture(const char *name);
		graphics::Image2dResourceData loadPngTexture(void *buffer);
	private:
		std::string getFileName(const char *name, llge::TextureQueryFormat queryFormat);
		void changeExt(std::string &str, const std::string &ext);
		bool equalsExt(const std::string &str, const std::string &ext);
		ImageInfo loadCompressedTextureSize(const char *name) const;
		static ImageInfo loadCompressedTextureSize(void *buffer, int size);
		graphics::Image2dResourceData loadCompressedTexture(const char *name);
		graphics::Image2dResourceData loadCompressedTexture(void *buffer, int size) const;
		ImageInfo loadPngTextureSize(const char *name);
		graphics::Image2dResourceData loadPngTexture();


		std::string _compressionExt;
		llge::TextureImage2dFormat _compressionFormat;
		int _loadingBufferSize;
		byte* _loadingBuffer;
		byte* _loadingHeaderBuffer;

		static std::string nilExt;
		static std::string pngExt;
		static std::string texExt;
		static std::string pvrExt;
		static std::string atcExt;
		static std::string pkmExt;
		static std::string etcExt;
		static std::string dxtExt;
		static unsigned int emptyPixel;
		
	};
}

#endif /*TEXTURES_LOADER_H*/