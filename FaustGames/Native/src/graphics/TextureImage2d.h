#ifndef TEXTURE_IMAGE_2D_H
#define TEXTURE_IMAGE_2D_H

#include "graphics_classes.h"
#include "TextureLoader.h"
#include "Texture.h"

namespace graphics
{
	class TexturesDecompressorBuffer
	{
	public:
		TexturesDecompressorBuffer();
		~TexturesDecompressorBuffer();
		uint* pixelsBuffer;
		int size;

		void realloc(int size);
	};



	class TextureImage2d : public Texture, public llge::ITextureImage2d
	{
	public:
		static int Size;
		static TextureImage2d * empty(){ return &_empty; }
		int _disposeCalls;
		TextureImage2d(bool generateMipmaps, bool useFilter);
        ~TextureImage2d() override;
		void setData(const Image2dData *data);
		void setData(const Image2dResourceData &data);
		void loadMipmaps(int width, int height, Image2dFormat::e format, unsigned* pixels);
		void reinitImage(bool generateMipmaps, bool useFilter);
		//void virtual setData(int width, int height, Image2dFormat::e format, unsigned int *pixels);
		static void createStatic();
		static void cleanupStatic();
		static bool isTextureFormatSupported(Image2dFormat::e format);

		virtual IntPtr API_CALL getTextureImageInstance() override;
		virtual ITexture* API_CALL getTexture() override;
		virtual void API_CALL LoadPixels(int width, int height, llge::TextureImage2dFormat format, void *pixels) override;
		virtual void API_CALL create() override;
		virtual void API_CALL cleanup() override;
		virtual void API_CALL dispose() override;
		virtual uint API_CALL getAlphaId() override;

		void disposeAlphaMap();
		void associate(TextureImage2d *value);
		void createAlphaIfNeeded();

		virtual int API_CALL getVerticesCount() OVERRIDE;
		virtual IntPtr API_CALL getVertices() OVERRIDE;
		virtual int API_CALL getIndicesCount() OVERRIDE;
		virtual IntPtr API_CALL getIndices() OVERRIDE;
		virtual bool API_CALL isAtlasEntry() OVERRIDE;

        static TextureImage2d _empty;
		bool AtlasEntry;

		static float getSize(int w, int h, Image2dFormat::e format);
	protected:
        bool _isDisposed;
	private:
		TextureImage2d();
		void setupConfig();
		void traceTriangles(int width, int height, Image2dFormat::e format, void *pixels);
		static GLenum getFormat(Image2dFormat::e format);
		static float getSize(int size, Image2dFormat::e format);
		static byte* getPixels(Image2dFormat::e format, uint *pixels);
		std::vector<core::Vector2> _tracedVertices;
		std::vector<ushort> _tracedIndices;
		bool _createMipmaps;
		bool _wrap;
		bool _filter;
		int _size;
        bool _created;
		TextureImage2d* _alphaMap;
    };

	class TextureAtlasPage : public TextureImage2d
	{
	public:
		TextureAtlasPage(bool useFilter);
		void createRect(float x, float y, float w, float h, TextureImage2d* texture);
        void reinitPage(bool useFilter);
		virtual void API_CALL dispose() override;
	private:
		std::vector<TextureImage2d* > _rects;
		int _aliveRects;
	};

	/*
	class TextureImage2dProxy : public Texture, public llge::ITextureImage2d
	{
	public:
		TextureImage2dProxy();
		~TextureImage2dProxy();
		virtual IntPtr API_CALL getTextureImageInstance(){ return this; }
		void setProxyInstance(TextureImage2d * instance);
		virtual ITexture* API_CALL getTexture();
		virtual void API_CALL LoadPixels(int width, int height, llge::TextureImage2dFormat format, void *pixels);
		virtual void API_CALL create();		
		virtual void API_CALL cleanup();
		virtual void API_CALL dispose();
	private:
		TextureImage2d * _instance;
		bool _isRealyProxy;
	};
     */

	void UpscaleToPot(const Image2dData *data, int potX, int potY, TexturesDecompressorBuffer *resultBuffer);
	void DecodeMortonOrder(const Image2dData *data, TexturesDecompressorBuffer *resultBuffer);
	int DecodeMortonPvrtc(const Image2dData *data, TexturesDecompressorBuffer *resultBuffer);
	int DecodePvrtc(const Image2dData *data, TexturesDecompressorBuffer *resultBuffer);
	void DecodeAtc(const Image2dData *data, TexturesDecompressorBuffer *resultBuffer, int &aW, int &aH);
	void DecodeEtc2(const Image2dData *data, TexturesDecompressorBuffer *resultBuffer, int &aW, int &aH);
	void DecodeEtc1(const Image2dData *data, TexturesDecompressorBuffer *resultBuffer, int &aW, int &aH);

	class TexturesPool
	{
	private:
		static std::map<TextureImage2d*, TextureImage2d*> _images;
		static std::map<TextureAtlasPage*, TextureAtlasPage*> _atlasPages;
	public:
		static TextureImage2d* GetImage(bool generateMipmaps = false, bool useFilter = true);
		static TextureAtlasPage* GetAtlasPage(bool useFilter = true);
		static void ReturnImage(TextureImage2d* image);
		static void ReturnAtlasPage(TextureAtlasPage* page);
	};
}


#endif /*TEXTURE_IMAGE_2D_H*/
