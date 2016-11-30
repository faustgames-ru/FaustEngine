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
		TextureImage2d(bool generateMipmaps, bool useFilter);
	
		void setData(const Image2dData *data);
		void loadMipmaps(int width, int height, Image2dFormat::e format, unsigned* pixels);
		//void virtual setData(int width, int height, Image2dFormat::e format, unsigned int *pixels);
		static void createStatic();
		static void cleanupStatic();
        virtual IntPtr API_CALL getTextureImageInstance(){ return this; }
		virtual ITexture* API_CALL getTexture(){ return this; }
		virtual void API_CALL LoadPixels(int width, int height, llge::TextureImage2dFormat format, void *pixels) override;
		virtual void API_CALL create() override;
		virtual void API_CALL cleanup();
		virtual void API_CALL dispose(){ delete this; }

		virtual int API_CALL getVerticesCount() OVERRIDE;
		virtual IntPtr API_CALL getVertices() OVERRIDE;
		virtual int API_CALL getIndicesCount() OVERRIDE;
		virtual IntPtr API_CALL getIndices() OVERRIDE;

        static TextureImage2d _empty;
		static bool TraceTriangles;
		bool AtlasEntry;
	protected:
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
    };

	class TextureAtlasPage : public TextureImage2d
	{
	public:
		TextureAtlasPage(bool useFilter);
		void createRect(float x, float y, float w, float h, TextureImage2d* texture);
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

	void DecodePvrtcOrder(const Image2dData *data, TexturesDecompressorBuffer *resultBuffer);
	int DecodePvrtc(const Image2dData *data, TexturesDecompressorBuffer *resultBuffer);
	void DecodeAtc(const Image2dData *data, TexturesDecompressorBuffer *resultBuffer, int &aW, int &aH);
	void DecodeEtc2(const Image2dData *data, TexturesDecompressorBuffer *resultBuffer, int &aW, int &aH);
	void DecodeEtc1(const Image2dData *data, TexturesDecompressorBuffer *resultBuffer, int &aW, int &aH);
}

#endif /*TEXTURE_IMAGE_2D_H*/
