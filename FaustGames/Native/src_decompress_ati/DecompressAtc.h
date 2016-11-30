#pragma once

namespace atc 
{
	typedef int BOOL;
	typedef unsigned char       CMP_BYTE;
	typedef unsigned long       CMP_DWORD;
	typedef unsigned long       DWORD;
	typedef unsigned char uint8;

	typedef enum _CODECError
	{
		CE_OK = 0,
		CE_Unknown,
		CE_Aborted,
	} CodecError;


#define BLOCK_SIZE_4X4X4    64
#define BLOCK_SIZE_4X4        16

#define RGBA8888_CHANNEL_A    3
#define RGBA8888_CHANNEL_R    2
#define RGBA8888_CHANNEL_G    1
#define RGBA8888_CHANNEL_B    0

#define RGBA8888_OFFSET_A (RGBA8888_CHANNEL_A * 8)
#define RGBA8888_OFFSET_R (RGBA8888_CHANNEL_R * 8)
#define RGBA8888_OFFSET_G (RGBA8888_CHANNEL_G * 8)
#define RGBA8888_OFFSET_B (RGBA8888_CHANNEL_B * 8)

#define BYTE_MASK 0x00ff

#define ATC_OFFSET_ALPHA 0
#define ATC_OFFSET_RGB 2

	class CCodec_ATC
	{
	public:
		static void DecompressRGBBlock(CMP_BYTE rgbBlock[BLOCK_SIZE_4X4X4], CMP_DWORD compressedBlock[2]);
		static void DecompressRGBABlock_ExplicitAlpha(CMP_BYTE rgbaBlock[BLOCK_SIZE_4X4X4], CMP_DWORD compressedBlock[4]);
		static void DecompressExplicitAlphaBlock(CMP_BYTE alphaBlock[BLOCK_SIZE_4X4], CMP_DWORD compressedBlock[2]);
	};

	class CCodec_ETC
	{
	public:
		static void DecompressRGBBlock(CMP_BYTE rgbBlock[BLOCK_SIZE_4X4X4], CMP_DWORD compressedBlock[2]);
		static void DecompressRGBABlock_ExplicitAlpha(CMP_BYTE rgbaBlock[BLOCK_SIZE_4X4X4], CMP_DWORD compressedBlock[4]);
		static void DecompressExplicitAlphaBlock(CMP_BYTE alphaBlock[BLOCK_SIZE_4X4], CMP_DWORD compressedBlock[2]);
	};

	typedef struct _Color888 {
		unsigned char red;
		unsigned char green;
		unsigned char blue;
	} Color888_t;
}