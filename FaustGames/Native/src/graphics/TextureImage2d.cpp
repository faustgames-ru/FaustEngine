#include "TextureImage2d.h"
#include "Uniforms.h"
#include "Errors.h"
#include "GraphicsDevice.h"
#include "Color.h"
#include "../../src_decompressors/PVRTDecompress.h"
#include "../../src_decompress_ati/DecompressAtc.h"
#include "../../src_etcpack/etcpack_lib.h"


#define ATC_RGB_AMD							0x8C92
#define ATC_RGBA_EXPLICIT_ALPHA_AMD			0x8C93
#define ATC_RGBA_INTERPOLATED_ALPHA_AMD		0x87EE

#define COMPRESSED_RGB_PVRTC_4BPPV1_IMG		0x8C00
#define COMPRESSED_RGB_PVRTC_2BPPV1_IMG		0x8C01
#define COMPRESSED_RGBA_PVRTC_4BPPV1_IMG	0x8C02
#define COMPRESSED_RGBA_PVRTC_2BPPV1_IMG	0x8C03

#define COMPRESSED_RGB8_ETC2                             0x9274
#define COMPRESSED_SRGB8_ETC2                            0x9275
#define COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2         0x9276
#define COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2        0x9277
#define COMPRESSED_RGBA8_ETC2_EAC                        0x9278
#define COMPRESSED_SRGB8_ALPHA8_ETC2_EAC                 0x9279
#define COMPRESSED_R11_EAC                               0x9270
#define COMPRESSED_SIGNED_R11_EAC                        0x9271
#define COMPRESSED_RG11_EAC                              0x9272
#define COMPRESSED_SIGNED_RG11_EAC                       0x9273

#define COMPRESSED_RGBA_ASTC_4x4_KHR            0x93B0
#define COMPRESSED_RGBA_ASTC_5x4_KHR            0x93B1
#define COMPRESSED_RGBA_ASTC_5x5_KHR            0x93B2
#define COMPRESSED_RGBA_ASTC_6x5_KHR            0x93B3
#define COMPRESSED_RGBA_ASTC_6x6_KHR            0x93B4
#define COMPRESSED_RGBA_ASTC_8x5_KHR            0x93B5
#define COMPRESSED_RGBA_ASTC_8x6_KHR            0x93B6
#define COMPRESSED_RGBA_ASTC_8x8_KHR            0x93B7
#define COMPRESSED_RGBA_ASTC_10x5_KHR           0x93B8
#define COMPRESSED_RGBA_ASTC_10x6_KHR           0x93B9
#define COMPRESSED_RGBA_ASTC_10x8_KHR           0x93BA
#define COMPRESSED_RGBA_ASTC_10x10_KHR          0x93BB
#define COMPRESSED_RGBA_ASTC_12x10_KHR          0x93BC
#define COMPRESSED_RGBA_ASTC_12x12_KHR          0x93BD

#define COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR    0x93D0
#define COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR    0x93D1
#define COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR    0x93D2
#define COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR    0x93D3
#define COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR    0x93D4
#define COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR    0x93D5
#define COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR    0x93D6
#define COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR    0x93D7
#define COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR   0x93D8
#define COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR   0x93D9
#define COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR   0x93DA
#define COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR  0x93DB
#define COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR  0x93DC
#define COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR  0x93DD

#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT			0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT		0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT		0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT		0x83F3

namespace graphics
{

	struct ImageColor
	{
		unsigned int value;
		ImageColor(unsigned int v)
		{
			value = v;
		}
		unsigned int getComponent(int i)
		{
			return (value >> (i * 8)) & 0xff;
		}
		void  setComponent(int i, int v)
		{
			value &= 0xff << (i * 8);
			value |= v << (i * 8);
		}
	};

	TextureImage2d::TextureImage2d() : _createMipmaps(false), _wrap(false), _filter(true)
	{
		setupConfig();
		_size = 0;
		_handle = 0;
		_handleDefault = _empty.getHandle();
	}

	void TextureImage2d::setupConfig()
	{
		_createMipmaps = GraphicsDevice::Default.config.generateMipmaps;
	}

	void TextureImage2d::traceTriangles(int width, int height, Image2dFormat::e format, void *pixels)
	{
		if (format != Image2dFormat::Rgba) return;
		int size = width*height;
		bool *boolPixels = new bool[size];
		uint *bmp = static_cast<uint *>(pixels);
		int blur = 4;
		int i = 0;
		for (; i < size; i++)
		{
			boolPixels[i] = false;
		}
		i = 0;
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				uint c = bmp[i];
				i++;
				unsigned char a = Color::getA(c);
				if (a > 16)
				{
					int sx = core::Math::max(x - blur, 0);
					int sy = core::Math::max(y - blur, 0);
					int fx = core::Math::min(x + blur, width - 1);
					int fy = core::Math::min(y + blur, height - 1);
					for (int y0 = sy; y0 <= fy; y0++)
					{
						for (int x0 = sx; x0 <= fx; x0++)
						{
							boolPixels[x0 + y0 * width] = true;
						}
					}
				}
			}
		}
		core::MarchingSquares marchingSquares;
		marchingSquares.build(boolPixels, width, height);
		marchingSquares.collectEdges();
		marchingSquares.simplifyPathes(8);
		marchingSquares.triangulatePathes();

		// todo: get triangles
		std::vector<core::MarchingSquaresPoint>& triangulationVertices = marchingSquares.getTriangulationVertices();;
		std::vector<ushort>& triangulationIndices = marchingSquares.getTriangulationIndices();
		_tracedVertices.resize(triangulationVertices.size());
		_tracedIndices.resize(triangulationIndices.size());
		for (i = 0; i < _tracedVertices.size(); i++)
		{
			_tracedVertices[i] = core::Vector2(triangulationVertices[i].x, triangulationVertices[i].y);
		}
		for (i = 0; i < _tracedIndices.size(); i++)
		{
			_tracedIndices[i] = triangulationIndices[i];
		}

		delete[] boolPixels;
	}

	TextureImage2d::TextureImage2d(bool generateMipmaps, bool useFilter) : _createMipmaps(generateMipmaps), _wrap(false), _filter(useFilter)
	{
		AtlasEntry = false;
		setupConfig();

		_size = 0;
		_handle = 0;
		_handleDefault = _empty.getHandle();
	}

	void API_CALL TextureImage2d::LoadPixels(int width, int height, llge::TextureImage2dFormat format, void *pixels)
	{
		if (AtlasEntry) return;
		Image2dData data;
		data.Width = width;
		data.Height = height;
		data.Format = static_cast<Image2dFormat::e>(format);
		data.Pixels = static_cast<unsigned int*>(pixels);
		setData(&data);
		data.Pixels = nullptr;
	}

	void API_CALL TextureImage2d::create()
	{
		if (AtlasEntry) return;
		//UniformValues::resetSamplers();
		glGenTextures(1, &_handle);
		Errors::check(Errors::GenTextures);
		GraphicsDevice::Default.resetSamplersState();
		glActiveTexture(GL_TEXTURE0 + GraphicsConstants::Samplers2DStart);
		Errors::check(Errors::ActiveTexture);
		glBindTexture(GL_TEXTURE_2D, _handle);
		Errors::check(Errors::BindTexture);
		if (_filter)
		{

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GraphicsDevice::Default.config.getMagFilter());
			Errors::check(Errors::TexParameteri);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GraphicsDevice::Default.config.getMinFilter());
			Errors::check(Errors::TexParameteri);
			/*
			if (_createMipmaps)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				Errors::check(Errors::TexParameteri);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				Errors::check(Errors::TexParameteri);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				Errors::check(Errors::TexParameteri);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				Errors::check(Errors::TexParameteri);
			}
			*/
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			Errors::check(Errors::TexParameteri);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			Errors::check(Errors::TexParameteri);
		}

		if (_wrap)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			Errors::check(Errors::TexParameteri);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			Errors::check(Errors::TexParameteri);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			Errors::check(Errors::TexParameteri);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			Errors::check(Errors::TexParameteri);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		Errors::check(Errors::BindTexture);
	}

	void API_CALL TextureImage2d::cleanup()
	{
		if (AtlasEntry) return;
		Size -= _size;
		_size = 0;
		glDeleteTextures(1, &_handle);
		Errors::check(Errors::DeleteTexture);
	}

	int TextureImage2d::getVerticesCount()
	{
		return _tracedVertices.size();
	}

	IntPtr TextureImage2d::getVertices()
	{
		return _tracedVertices.data();
	}

	int TextureImage2d::getIndicesCount()
	{
		return _tracedIndices.size();
	}

	IntPtr TextureImage2d::getIndices()
	{
		return _tracedIndices.data();
	}

	bool TextureImage2d::isAtlasEntry()
	{
		return AtlasEntry;
	}

	struct PvrtcBlock
	{
		int i0;
		int i1;
	};

	void TextureImage2d::loadMipmaps(int width, int height, Image2dFormat::e format, unsigned int *pixels)
	{
		int w = width;
		int h = height;

		int mip = 0;
		while ((w > 0) || (h > 0))
		{
			glTexImage2D(GL_TEXTURE_2D, mip, getFormat(format), w == 0 ? 1 : w, h == 0 ? 1 : h, 0, getFormat(format), GL_UNSIGNED_BYTE, pixels);
			Errors::check(Errors::TexImage2D);
			int pw = w;
			w /= 2;
			h /= 2;

			int i0 = 0;
			int i1 = 0;
			int i2 = 0;
			int i3 = 0;
			ImageColor c0(0);
			ImageColor c1(0);
			ImageColor c2(0);
			ImageColor c3(0);
			ImageColor c(0);

			unsigned int r;
			unsigned int g;
			unsigned int b;
			unsigned int a;

			if (format == Image2dFormat::Rgba)
			{
				for (int y = 0; y < h; y++)
				{
					i0 = (y * 2 + 0) * pw + 0 + 0;
					i1 = (y * 2 + 1) * pw + 0 + 0;
					i2 = (y * 2 + 0) * pw + 0 + 1;
					i3 = (y * 2 + 1) * pw + 0 + 1;

					for (int x = 0; x < w; x++)
					{
						//i0 = (y * 2 + 0) * w * 2 + x * 2 + 0;
						//i1 = (y * 2 + 1) * w * 2 + x * 2 + 0;
						//i2 = (y * 2 + 0) * w * 2 + x * 2 + 1;
						//i3 = (y * 2 + 1) * w * 2 + x * 2 + 1;



						c0 = ImageColor(pixels[i0]);
						c1 = ImageColor(pixels[i1]);
						c2 = ImageColor(pixels[i2]);
						c3 = ImageColor(pixels[i3]);


						r = (c0.getComponent(0) + c1.getComponent(0) + c2.getComponent(0) + c3.getComponent(0)) >> 2;
						g = (c0.getComponent(1) + c1.getComponent(1) + c2.getComponent(1) + c3.getComponent(1)) >> 2;
						b = (c0.getComponent(2) + c1.getComponent(2) + c2.getComponent(2) + c3.getComponent(2)) >> 2;
						a = (c0.getComponent(3) + c1.getComponent(3) + c2.getComponent(3) + c3.getComponent(3)) >> 2;
						c.setComponent(0, r);
						c.setComponent(1, g);
						c.setComponent(2, b);
						c.setComponent(3, a);

						pixels[y * w + x] = c0.value;

						i0 += 2;
						i1 += 2;
						i2 += 2;
						i3 += 2;
					}
				}
				mip++;
			}
			else
			{
				byte* pixels3 = static_cast<byte*>(static_cast<void *>(pixels));
				for (int y = 0; y < h; y++)
				{
					i0 = ((y * 2 + 0) * pw + 0 + 0) * 3;
					i1 = ((y * 2 + 1) * pw + 0 + 0) * 3;
					i2 = ((y * 2 + 0) * pw + 0 + 1) * 3;
					i3 = ((y * 2 + 1) * pw + 0 + 1) * 3;

					for (int x = 0; x < w; x++)
					{
						r = (pixels3[i0 + 0] + pixels3[i1 + 0] + pixels3[i2 + 0] + pixels3[i3 + 0]) >> 2;
						g = (pixels3[i0 + 1] + pixels3[i1 + 1] + pixels3[i2 + 1] + pixels3[i3 + 1]) >> 2;
						b = (pixels3[i0 + 2] + pixels3[i1 + 2] + pixels3[i2 + 2] + pixels3[i3 + 2]) >> 2;

						pixels3[(y * w + x) * 3 + 0] = r;
						pixels3[(y * w + x) * 3 + 1] = g;
						pixels3[(y * w + x) * 3 + 2] = b;

						i0 += 2 * 3;
						i1 += 2 * 3;
						i2 += 2 * 3;
						i3 += 2 * 3;
					}
				}
				mip++;
			}
		}
	}

	struct PVRTCWord
	{
		uint u32ModulationData;
		uint u32ColorData;
	};
	
	std::vector<int> _formats;

	bool hasExt(const char* exts, const char* ext)
	{
		return strstr(exts, ext) != NULL;
	}

	std::string atc_ext1("GL_AMD_compressed_ATC_texture");
	std::string pvr_ext1("GL_IMG_texture_compression_pvrtc");

	void TextureImage2d::setData(const Image2dData *data)
	{
		if (AtlasEntry) return;
		Size -= _size;
		_size = getSize(data->Width * data->Height, data->Format);
		Size += _size;
		UniformValues::resetSamplers();
		GraphicsDevice::Default.resetSamplersState();

		glActiveTexture(GL_TEXTURE0);
		Errors::check(Errors::ActiveTexture);

		glBindTexture(GL_TEXTURE_2D, _handle);
		Errors::check(Errors::BindTexture);

		float border = static_cast<float>(data->BorderSize);


		if (_createMipmaps && (data->Format == Image2dFormat::Rgb || data->Format == Image2dFormat::Rgba))
		{
			loadMipmaps(data->Width, data->Height, data->Format, data->Pixels);
		}
		else
		{
			if (data->Format == Image2dFormat::Rgb || data->Format == Image2dFormat::Rgba)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, getFormat(data->Format), data->Width, data->Height, 0, getFormat(data->Format), GL_UNSIGNED_BYTE, data->Pixels);
				Errors::check(Errors::TexImage2D);
			}
			else if (data->Format == Image2dFormat::Rgba4444)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, getFormat(data->Format), data->Width, data->Height, 0, getFormat(data->Format), GL_UNSIGNED_SHORT_4_4_4_4, data->Pixels + data->RawDataOffset);
				Errors::check(Errors::TexImage2D);
			}
			else
			{
				GLint numFormats = 0;
				glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &numFormats);
				_formats.resize(numFormats);
				glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, _formats.data());
				int f = getFormat(data->Format);
				bool isFormatSupported = false;
				
			
				for (int i = 0; i < _formats.size(); i++)
				{
					if (f == _formats[i])
					{
						isFormatSupported = true;
					}
				}
				/*
				const char* exts = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
				switch(data->Format)
				{
				case Image2dFormat::Rgba: break;
				case Image2dFormat::Rgb: break;
				case Image2dFormat::Pvrtc12: break;
				case Image2dFormat::Pvrtc14: break;
				case Image2dFormat::Etc1: break;
				case Image2dFormat::Etc2: break;
				case Image2dFormat::Rgba4444: break;
				case Image2dFormat::Atc: 
					isFormatSupported = hasExt(exts, atc_ext1.c_str());
					break;					
				case Image2dFormat::Astc: break;
				default: break;
				}
				*/
				//__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", exts);
				if (!isFormatSupported)
				{
					//__android_log_print(ANDROID_LOG_ERROR, "TRACKERS", "%s", "format not supported");

					if (data->Format == Image2dFormat::Pvrtc14 || data->Format == Image2dFormat::Pvrtc12)
					{
						TexturesDecompressorBuffer resultBuffer;
						int pot = 0;
						if (data->BlocksOrder == Image2dBlocksOrder::Morton)
						{
							pot = DecodePvrtc(data, &resultBuffer);
						}
						else
						{
							pot = DecodeMortonPvrtc(data, &resultBuffer);
						}
				
						Image2dFormat::e fmt = Image2dFormat::Rgba;

						glTexImage2D(GL_TEXTURE_2D, 0, getFormat(fmt), pot, pot, 0, getFormat(fmt), GL_UNSIGNED_BYTE, resultBuffer.pixelsBuffer);
						transform = TextureTransform(
							border / static_cast<float>(pot), 
							border / static_cast<float>(pot),
							static_cast<float>(data->Width) / static_cast<float>(pot),
							static_cast<float>(data->Height) / static_cast<float>(pot));
						Errors::check(Errors::TexImage2D);
					}
					else if (data->Format == Image2dFormat::Atc)
					{
						TexturesDecompressorBuffer pixelsBuffer;
						int aW = data->Width;
						int aH = data->Height;
						DecodeAtc(data, &pixelsBuffer, aW, aH);
						Image2dFormat::e fmt = Image2dFormat::Rgba;

						glTexImage2D(GL_TEXTURE_2D, 0, getFormat(fmt), aW, aH, 0, getFormat(fmt), GL_UNSIGNED_BYTE, pixelsBuffer.pixelsBuffer);
						transform = TextureTransform(
							static_cast<float>(border) / static_cast<float>(aW), 
							static_cast<float>(border) / static_cast<float>(aH),
							static_cast<float>(data->Width) / static_cast<float>(aW),
							static_cast<float>(data->Height) / static_cast<float>(aH));
						Errors::check(Errors::TexImage2D);
					}
					/*else if (data->Format == Image2dFormat::Etc1)
					{
						TexturesDecompressorBuffer pixelsBuffer;
						int aW = data->Width;
						int aH = data->Height;
						DecodeEtc1(data, &pixelsBuffer, aW, aH);
						Image2dFormat::e fmt = Image2dFormat::Rgb;

						glTexImage2D(GL_TEXTURE_2D, 0, getFormat(fmt), aW, aH, 0, getFormat(fmt), GL_UNSIGNED_BYTE, pixelsBuffer.pixelsBuffer);
						transform = TextureTransform(0, 0,
							static_cast<float>(data->Width) / static_cast<float>(aW),
							static_cast<float>(data->Height) / static_cast<float>(aH));
						Errors::check(Errors::TexImage2D);
					}*/
					
					else if (data->Format == Image2dFormat::Etc2)
					{
						TexturesDecompressorBuffer pixelsBuffer;
						int aW = data->Width;
						int aH = data->Height;
						DecodeEtc2(data, &pixelsBuffer, aW, aH);
						Image2dFormat::e fmt = Image2dFormat::Rgba;

						glTexImage2D(GL_TEXTURE_2D, 0, getFormat(fmt), aW, aH, 0, getFormat(fmt), GL_UNSIGNED_BYTE, pixelsBuffer.pixelsBuffer);
						transform = TextureTransform(
							static_cast<float>(border) / static_cast<float>(aW),
							static_cast<float>(border) / static_cast<float>(aH),
							static_cast<float>(data->Width) / static_cast<float>(aW),
							static_cast<float>(data->Height) / static_cast<float>(aH));						Errors::check(Errors::TexImage2D);
					}
					
					else if (data->Format == Image2dFormat::Dxt)
					{
						// todo: Astc decode
					}
					// todo convert	
				}
				if (isFormatSupported)
				{
					if ((data->Format == Image2dFormat::Pvrtc12 || data->Format == Image2dFormat::Pvrtc14))
					{
						int pot = core::Math::pot(core::Math::max(data->Width + border * 2, data->Height + border * 2));
						if (data->BlocksOrder != Image2dBlocksOrder::Morton)
						{
							//if (pot == data->Width && pot == data->Height)
							{
								TexturesDecompressorBuffer pixelsBuffer;
								DecodeMortonOrder(data, &pixelsBuffer);
								int compressedImageSize = getSize(pot*pot, data->Format);
								glCompressedTexImage2D(GL_TEXTURE_2D, 0, getFormat(data->Format), pot, pot, 0, compressedImageSize, pixelsBuffer.pixelsBuffer);
								Errors::check(Errors::CompressedTexImage2D);
							}
						}
						else
						{
							int compressedImageSize = getSize(pot*pot, data->Format);
							glCompressedTexImage2D(GL_TEXTURE_2D, 0, getFormat(data->Format), pot, pot, 0, compressedImageSize, data->Pixels + data->RawDataOffset);
							Errors::check(Errors::CompressedTexImage2D);
						}
						transform = TextureTransform(
							border / static_cast<float>(pot),
							border / static_cast<float>(pot),
							static_cast<float>(data->Width) / static_cast<float>(pot),
							static_cast<float>(data->Height) / static_cast<float>(pot));
						return;
					}
					int compressedImageSize = getSize(data->Width + border * 2, data->Height + border * 2, data->Format);
					glCompressedTexImage2D(GL_TEXTURE_2D, 0, getFormat(data->Format), data->Width + border*2, data->Height + border*2, 0, compressedImageSize, data->Pixels + data->RawDataOffset);
					transform = TextureTransform(
						border / static_cast<float>(data->Width + border*2),
						border / static_cast<float>(data->Height + border*2),
						static_cast<float>(data->Width) / static_cast<float>(data->Width + border*2),
						static_cast<float>(data->Height) / static_cast<float>(data->Height + border*2));
					Errors::check(Errors::CompressedTexImage2D);
				}
			}

		}
		glBindTexture(GL_TEXTURE_2D, 0);
		Errors::check(Errors::BindTexture);
	}

	void UpscaleToPot(const Image2dData *data, int potX, int potY, TexturesDecompressorBuffer *resultBuffer)
	{
		int size = potX * potY;
		resultBuffer->realloc(size);
		int64_t* dst = reinterpret_cast<int64_t*>(resultBuffer->pixelsBuffer);
		int64_t* src = reinterpret_cast<int64_t*>(data->Pixels + data->RawDataOffset);
		int blocksX = core::Math::align(data->Width, 4) / 4;
		int blocksY = core::Math::align(data->Height, 4) / 4;
		int potBlocksX = core::Math::align(potX, 4) / 4;

		for (int y = 0; y < blocksY; y++)
		{
			int64_t* dstRow = dst;
			int64_t* srcRow = src;
			for (int x = 0; x < blocksX; x++)
			{
				*dstRow = *srcRow; 
				dstRow++;
				srcRow++;
				*dstRow = *srcRow;
				dstRow++;
				srcRow++;
			}
			dst += potBlocksX*2;
			src += blocksX*2;
		}
	}
	
	void DecodeMortonOrder(const Image2dData *data, TexturesDecompressorBuffer *resultBuffer)
	{
		int pot = core::Math::pot(core::Math::max(data->Width + data->BorderSize*2, data->Height + data->BorderSize*2));

		int64_t* src = reinterpret_cast<int64_t*>(data->Pixels + data->RawDataOffset);

		int blocksX = core::Math::align(data->Width + data->BorderSize*2, 4) / 4;
		if (data->Format == Image2dFormat::Pvrtc12)
			blocksX = core::Math::align(data->Width + data->BorderSize*2, 8) / 8;
		int blocksY = core::Math::align(data->Height + data->BorderSize*2, 4) / 4;

		int size = pot * pot;
		if (data->Format == Image2dFormat::Pvrtc12)
			size /= 4;
		else
			size /= 2;
		resultBuffer->realloc(size);
		int64_t* dst = reinterpret_cast<int64_t*>(resultBuffer->pixelsBuffer);
		int i = 0;
		for (int y = 0; y < blocksY; y++)
		{
			for (int x = 0; x < blocksX; x++)
			{
				int code = core::Math::mortonCode(y, x);
				int64_t block = src[i];
				dst[code] = block;
				i++;
			}
		}
	}

	int DecodeMortonPvrtc(const Image2dData* data, TexturesDecompressorBuffer* resultBuffer)
	{
		int pot = core::Math::pot(core::Math::max(data->Width + data->BorderSize * 2, data->Height + data->BorderSize * 2));
		TexturesDecompressorBuffer pixelsBuffer;

		pixelsBuffer.realloc(pot * pot);
		resultBuffer->realloc(pot * pot);
		int64_t* dst = reinterpret_cast<int64_t*>(pixelsBuffer.pixelsBuffer);
		int64_t* src = reinterpret_cast<int64_t*>(data->Pixels + data->RawDataOffset);
		for (int i = 0; i < pixelsBuffer.size; i++)
		{
			pixelsBuffer.pixelsBuffer[i] = 0;
		}

		int blocksX = core::Math::align(data->Width + data->BorderSize * 2, 4) / 4;
		if (data->Format == Image2dFormat::Pvrtc12)
			blocksX = core::Math::align(data->Width + data->BorderSize * 2, 8) / 8;
		int blocksY = core::Math::align(data->Height + data->BorderSize * 2, 4) / 4;
		int i = 0;
		for (int y = 0; y < blocksY; y++)
		{
			for (int x = 0; x < blocksX; x++)
			{
				int code = core::Math::mortonCode(y, x);
				int64_t block = src[i];
				dst[code] = block;
				i++;
			}
		}

		pvr::PVRTDecompressPVRTC(dst, data->Format == Image2dFormat::Pvrtc12 ? 1 : 0, pot, pot, reinterpret_cast<unsigned char*>(resultBuffer->pixelsBuffer));
		return pot;
	}

	int DecodePvrtc(const Image2dData *data, TexturesDecompressorBuffer *resultBuffer)
	{
		int pot = core::Math::pot(core::Math::max(data->Width + data->BorderSize*2, data->Height + data->BorderSize * 2));
		if (pot != data->Height || pot != data->Width) return pot;
		resultBuffer->realloc(pot * pot);
		pvr::PVRTDecompressPVRTC(data->Pixels + data->RawDataOffset, data->Format == Image2dFormat::Pvrtc12 ? 1 : 0, pot, pot, reinterpret_cast<unsigned char*>(resultBuffer->pixelsBuffer));
		return pot;
	}

	void DecodeAtc(const Image2dData* data, TexturesDecompressorBuffer* pixelsBuffer, int& aW, int& aH)
	{
		aW = core::Math::align(data->Width + data->BorderSize*2, 4);
		aH = core::Math::align(data->Height + data->BorderSize * 2, 4);

		pixelsBuffer->realloc(aW * aH);

		int blocksX = aW / 4;
		int blocksY = aH / 4;

		uint* dst = reinterpret_cast<uint*>(pixelsBuffer->pixelsBuffer);
		unsigned long* src = reinterpret_cast<unsigned long*>(data->Pixels + data->RawDataOffset);
		int srcIndex = 0;
		int dstBlockIndex = 0;


		for (int y = 0; y < blocksY; y++)
		{
			uint* row = dst;
			for (int x = 0; x < blocksX; x++)
			{
				unsigned long compressed[4];
				byte rgba[4 * 4 * 4];
				uint* rgbaptr = reinterpret_cast<uint*>(rgba);
				compressed[0] = src[srcIndex];
				srcIndex++;
				compressed[1] = src[srcIndex];
				srcIndex++;
				compressed[2] = src[srcIndex];
				srcIndex++;
				compressed[3] = src[srcIndex];
				srcIndex++;

				atc::CCodec_ATC::DecompressRGBABlock_ExplicitAlpha(rgba, compressed);

				int j = 0;
				uint* subDst = row;
				for (int iy = 0; iy < 4; iy++)
				{
					uint* subRow = subDst;
					for (int ix = 0; ix < 4; ix++)
					{
						*subRow = *rgbaptr;
						subRow++;
						rgbaptr++;
					}
					subDst += aW;
				}
				row += 4;
			}
			dst += aW * 4;
		}
	}
	
	void copyBytes4(byte* src, byte* dst)
	{
		dst[0] = src[0];
		dst[1] = src[1];
		dst[2] = src[2];
		dst[3] = src[3];
	}
	
	void swapBytes4(byte* src, byte* dst)
	{
		dst[3] = src[0];
		dst[2] = src[1];
		dst[1] = src[2];
		dst[0] = src[3];
	}
	int swapBytes4(int value)
	{
		
		byte* src = reinterpret_cast<byte*>(&value);
		int result;
		byte* dst = reinterpret_cast<byte*>(&result);
		swapBytes4(src, dst);
		return result;
	}
	
	void DecodeEtc2(const Image2dData* data, TexturesDecompressorBuffer* pixelsBuffer, int& aW, int& aH)
	{
		setupAlphaTable();

		aW = core::Math::align(data->Width + data->BorderSize*2, 4);
		aH = core::Math::align(data->Height + data->BorderSize * 2, 4);

		pixelsBuffer->realloc(aW * aH);

		int blocksX = aW / 4;
		int blocksY = aH / 4;

		byte* dst = reinterpret_cast<byte*>(pixelsBuffer->pixelsBuffer);
		byte* src = reinterpret_cast<byte*>(data->Pixels + data->RawDataOffset);
		int srcIndex = 0;


		unsigned int compressed[4];
		for (int y = 0; y < blocksY; y++)
		{
			for (int x = 0; x < blocksX; x++)
			{
				byte* compressedPtr0 = reinterpret_cast<byte*>(compressed + 0);
				byte* compressedPtr1 = reinterpret_cast<byte*>(compressed + 1);
				byte* compressedPtr2 = reinterpret_cast<byte*>(compressed + 2);
				byte* compressedPtr3 = reinterpret_cast<byte*>(compressed + 3);
				copyBytes4(src + srcIndex, compressedPtr0);
				srcIndex += 4;
				copyBytes4(src + srcIndex, compressedPtr1);
				srcIndex += 4;
				swapBytes4(src + srcIndex, compressedPtr2);
				srcIndex += 4;
				swapBytes4(src + srcIndex, compressedPtr3);
				srcIndex += 4;
				
				decompressBlockAlphaC(compressedPtr0, dst + 3, aW, aH, x * 4, y * 4, 4);
				decompressBlockETC2c(compressed[2], compressed[3], dst, aW, aH, x * 4, y * 4, 4);
			}
		}
	}
	
	void DecodeEtc1(const Image2dData* data, TexturesDecompressorBuffer* pixelsBuffer, int& aW, int& aH)
	{
		aW = core::Math::align(data->Width, 4);
		aH = core::Math::align(data->Height, 4);

		pixelsBuffer->realloc(aW * aH * 3 / 4);

		int blocksX = aW / 4;
		int blocksY = aH / 4;

		byte* dst = reinterpret_cast<byte*>(pixelsBuffer->pixelsBuffer);
		byte* src = reinterpret_cast<byte*>(data->Pixels + data->RawDataOffset);
		int srcIndex = 0;


		unsigned int compressed[2];
		for (int y = 0; y < blocksY; y++)
		{
			for (int x = 0; x < blocksX; x++)
			{
				byte* compressedPtr0 = reinterpret_cast<byte*>(compressed+0);
				byte* compressedPtr1 = reinterpret_cast<byte*>(compressed+1);
				swapBytes4(src + srcIndex, compressedPtr0);
				srcIndex += 4;
				swapBytes4(src + srcIndex, compressedPtr1);
				srcIndex += 4;

				decompressBlockETC2(compressed[0], compressed[1], dst, aW, aH, x*4, y*4);
			}
		}
	}
	
	GLenum TextureImage2d::getFormat(Image2dFormat::e format)
	{
		switch (format)
		{
		case Image2dFormat::Rgb:
			return GL_RGB;
		case Image2dFormat::Rgba:
			return GL_RGBA;
		case Image2dFormat::Pvrtc12:
			return COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
		case Image2dFormat::Pvrtc14:
			return COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
		case Image2dFormat::Etc1:
			return COMPRESSED_RGB8_ETC2;
		case Image2dFormat::Etc2:
			return COMPRESSED_RGBA8_ETC2_EAC;
		case Image2dFormat::Dxt:
			return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		case Image2dFormat::Rgba4444:
			return GL_RGBA;
		default:
			return GL_RGBA;
		}
	}

	float TextureImage2d::getSize(int size, Image2dFormat::e format)
	{
		switch (format)
		{
		case Image2dFormat::Rgb:
			return size*3;
		case Image2dFormat::Rgba:
			return size*4;
		case Image2dFormat::Pvrtc12:
			return size / 4;
		case Image2dFormat::Pvrtc14:
			return size / 2;
		case Image2dFormat::Etc1:
			return size / 2;
		case Image2dFormat::Etc2:
		case Image2dFormat::Dxt:
		case Image2dFormat::Atc:
			return size;
		default:
			return size;
		}
	}

	float TextureImage2d::getSize(int w, int h, Image2dFormat::e format)
	{
		switch (format)
		{
		case Image2dFormat::Rgb:
			return w*h*3;
		case Image2dFormat::Rgba:
			return w*h*4;
		case Image2dFormat::Pvrtc12:
			return core::Math::align(w,4)*core::Math::align(h, 4) /4;
		case Image2dFormat::Pvrtc14:
			return core::Math::align(w, 4)*core::Math::align(h, 4) / 2;
		case Image2dFormat::Etc1:
			return core::Math::align(w, 4)*core::Math::align(h, 4) / 2;
		case Image2dFormat::Etc2:
		case Image2dFormat::Dxt:
		case Image2dFormat::Atc:
			return core::Math::align(w, 4)*core::Math::align(h, 4);
		default:
			return core::Math::align(w, 4)*core::Math::align(h, 4) / 2;
		}
	}

	byte* TextureImage2d::getPixels(Image2dFormat::e format, uint* pixels)
	{
		return reinterpret_cast<byte*>(pixels);
	}

	TexturesDecompressorBuffer::TexturesDecompressorBuffer()
	{
		size = 0;
		pixelsBuffer = nullptr;
	}

	TexturesDecompressorBuffer::~TexturesDecompressorBuffer()
	{
		if (pixelsBuffer == nullptr) return;
		delete[] pixelsBuffer;
		size = 0;
		pixelsBuffer = nullptr;
	}

	void TexturesDecompressorBuffer::realloc(int newSize)
	{
		if(pixelsBuffer == nullptr)
		{
			size = newSize;
			pixelsBuffer = new uint[size];
		}
		else
		{
			if (newSize > size)
			{
				delete[] pixelsBuffer;
				size = newSize;
				if (newSize > 0)
				{
					pixelsBuffer = new uint[size];
				}
				else
				{
					pixelsBuffer = nullptr;
				}
			}
		}
	}

	TextureAtlasPage::TextureAtlasPage(bool useFilter): TextureImage2d(false, useFilter), _aliveRects(0)
	{
	}

	void TextureAtlasPage::createRect(float x, float y, float w, float h, TextureImage2d* result)
	{
		result->setHandle(getHandle());
		result->transform = TextureTransform(x, y, w, h);
		result->AtlasEntry = true;
		_rects.push_back(result);
		_aliveRects++;
	}

	void TextureImage2d::createStatic()
	{
		_empty.create();
		unsigned int blackPixel = 0x00000000;
		Image2dData data;
		data.Width = 1;
		data.Height = 1;
		data.Format = Image2dFormat::Rgba;
		data.Pixels = &blackPixel;
		_empty.setData(&data);
		data.Pixels = nullptr;
	}

	void TextureImage2d::cleanupStatic()
	{
		_empty.cleanup();
	}

	int TextureImage2d::Size(0);

	TextureImage2d TextureImage2d::_empty;
	bool TextureImage2d::TraceTriangles(
#ifdef __ANDROID__
		false
#else
#ifdef __APPLE__
		false
#else	
		true
#endif
#endif
		);
	/*
	TextureImage2dProxy::TextureImage2dProxy()
	{
		_handle = 0;
		_instance = 0;
		_handleDefault = TextureImage2d::_empty.getHandle();
		_isRealyProxy = false;
		X = 0;
		Y = 0;
		W = 1;
		H = 1;
	}
	TextureImage2dProxy::~TextureImage2dProxy()
	{
		if (_isRealyProxy) return;
		if (_instance)
		{
			_instance->dispose();
		}
	}

	void TextureImage2dProxy::setProxyInstance(TextureImage2d * instance)
	{
		_isRealyProxy = true;
		_instance = instance;
		_handle = _instance->getId();
	}

	llge::ITexture* API_CALL TextureImage2dProxy::getTexture()
	{
		return this;
	}

	void API_CALL TextureImage2dProxy::LoadPixels(int width, int height, llge::TextureImage2dFormat format, void *pixels)
	{
		if (_isRealyProxy) return;
		if (!_instance)
        {
			_instance = new TextureImage2d(false, true);
            _instance->create();
        }
        _handle = _instance->getId();
		_instance->LoadPixels(width, height, format, pixels);
	}

	void API_CALL TextureImage2dProxy::create()	{ }

	void API_CALL TextureImage2dProxy::cleanup()
	{
		if (_isRealyProxy) return;
		if (_instance)
		{
			_instance->cleanup();
		}
	}

	void API_CALL TextureImage2dProxy::dispose()
	{
		delete this;
	}
 */

	/*
	* Color1555To888() - In addition to the conversion, this function returns a boolean
	*                    TRUE or FALSE indicating if the high bit is set.
	*/
	

}
