#include "DecompressAtc.h"

namespace atc
{
	static BOOL
		Color1555To888(unsigned int color1555, Color888_t *pColor888Ret)
	{
		BOOL boolBlackTrick = color1555 & 0x00008000;

		pColor888Ret->red = (unsigned char)(((color1555 & 0x00007C00) >> 7) | ((color1555 & 0x00007000) >> 12));
		pColor888Ret->green = (unsigned char)(((color1555 & 0x000003E0) >> 2) | ((color1555 & 0x00000380) >> 7));
		pColor888Ret->blue = (unsigned char)(((color1555 & 0x0000001F) << 3) | ((color1555 & 0x0000001C) >> 2));

		return boolBlackTrick;
	}



	static void
		Color565To888(unsigned int color565, Color888_t *pColor888Ret)
	{
		pColor888Ret->red = (unsigned char)(((color565 & 0x0000F800) >> 8) | ((color565 & 0x0000E000) >> 13));
		pColor888Ret->green = (unsigned char)(((color565 & 0x000007E0) >> 3) | ((color565 & 0x00000600) >> 9));
		pColor888Ret->blue = (unsigned char)(((color565 & 0x0000001F) << 3) | ((color565 & 0x0000001C) >> 2));
	}

#ifndef max
#define max(A,B) ((A) > (B) ? (A) : (B))
#endif

	static void
		DeriveMedLowFromHighAndMedHighColors(Color888_t *pColorHigh,
			Color888_t *pColorMedHigh,
			Color888_t *pOut_ColorMedLow,    // Return value
			Color888_t *pOut_ColorLow        // Return value
		)
	{
		// Don't go negative!!!

		pOut_ColorMedLow->red = (unsigned char)max((signed int)pColorMedHigh->red - (pColorHigh->red >> 2), 0);
		pOut_ColorMedLow->green = (unsigned char)max((signed int)pColorMedHigh->green - (pColorHigh->green >> 2), 0);
		pOut_ColorMedLow->blue = (unsigned char)max((signed int)pColorMedHigh->blue - (pColorHigh->blue >> 2), 0);

		pOut_ColorLow->red = 0;
		pOut_ColorLow->green = 0;
		pOut_ColorLow->blue = 0;
	}




	static void
		DeriveMiddleColors(Color888_t *pColorHigh,
			Color888_t *pOut_ColorMedHigh,   // Return value
			Color888_t *pOut_ColorMedLow,    // Return value
			Color888_t *pColorLow
		)
	{
		pOut_ColorMedHigh->red = (unsigned char)(((unsigned int)pColorHigh->red * 5 + (unsigned int)pColorLow->red * 3) >> 3);
		pOut_ColorMedHigh->green = (unsigned char)(((unsigned int)pColorHigh->green * 5 + (unsigned int)pColorLow->green * 3) >> 3);
		pOut_ColorMedHigh->blue = (unsigned char)(((unsigned int)pColorHigh->blue * 5 + (unsigned int)pColorLow->blue * 3) >> 3);

		pOut_ColorMedLow->red = (unsigned char)(((unsigned int)pColorHigh->red * 3 + (unsigned int)pColorLow->red * 5) >> 3);
		pOut_ColorMedLow->green = (unsigned char)(((unsigned int)pColorHigh->green * 3 + (unsigned int)pColorLow->green * 5) >> 3);
		pOut_ColorMedLow->blue = (unsigned char)(((unsigned int)pColorHigh->blue * 3 + (unsigned int)pColorLow->blue * 5) >> 3);
	}

	static void
		SetDecoderColors(unsigned int colorLow565or1555,       // IN:  Encoded low color
			unsigned int colorHigh565,            // IN:  Encoded high color
			Color888_t(*pDecoderColors888)[4]  // OUT: L, ML, MH, H colors to use
		)
	{
		static int      iColorLow = 0;
		static int      iColorMedLow = 1;
		static int      iColorMedHigh = 2;
		static int      iColorHigh = 3;

		//----------------------------------------------------------
		// Set colors based on encoder specified by ATITC SIGNATURE
		//----------------------------------------------------------

		BOOL boolBlackTrick = Color1555To888(colorLow565or1555, &(*pDecoderColors888)[iColorLow]);
		Color565To888(colorHigh565, &(*pDecoderColors888)[iColorHigh]);

		if (boolBlackTrick)
		{
			(*pDecoderColors888)[iColorMedHigh] = (*pDecoderColors888)[iColorLow];

			DeriveMedLowFromHighAndMedHighColors(&(*pDecoderColors888)[iColorHigh],
				&(*pDecoderColors888)[iColorMedHigh],
				&(*pDecoderColors888)[iColorMedLow],
				&(*pDecoderColors888)[iColorLow]);
		}
		else
		{
			DeriveMiddleColors(&(*pDecoderColors888)[iColorHigh],
				&(*pDecoderColors888)[iColorMedHigh],
				&(*pDecoderColors888)[iColorMedLow],
				&(*pDecoderColors888)[iColorLow]);
		}

	}


	void atiDecodeRGBBlockATITC(
		Color888_t(*pPixelsOut)[4][4],
		unsigned int    bitIndices,
		unsigned int    colorLow565or1555,
		unsigned int    colorHigh565
	)
	{
		int             row, col;
		Color888_t      aColor888[4];
		static int      iColorLow = 0;
		static int      iColorMedLow = 1;
		static int      iColorMedHigh = 2;
		static int      iColorHigh = 3;

		//--------------------
		//  Setup the colors  
		//--------------------

		SetDecoderColors(colorLow565or1555, colorHigh565, &aColor888);


		//--------------------
		//  Decode the block
		//--------------------

		for (row = 0; row < 4; row++)
		{
			for (col = 0; col < 4; col++)
			{
				(*pPixelsOut)[row][col].red = aColor888[bitIndices & 0x00000003].red;
				(*pPixelsOut)[row][col].green = aColor888[bitIndices & 0x00000003].green;
				(*pPixelsOut)[row][col].blue = aColor888[bitIndices & 0x00000003].blue;

				bitIndices >>= 2;
			}
		}
	}
#define UNREFERENCED_PARAMETER(P)          (P)
#define GETBITSHIGH(source, size, startpos)  (( (source) >> (((startpos)-32)-(size)+1) ) & ((1<<(size)) -1))
#define GETBITS(source, size, startpos)  (( (source) >> ((startpos)-(size)+1) ) & ((1<<(size)) -1))
#define RED_CHANNEL(img,width,x,y,channels)   img[channels*(y*width+x)+0]
#define GREEN_CHANNEL(img,width,x,y,channels) img[channels*(y*width+x)+1]
#define BLUE_CHANNEL(img,width,x,y,channels)  img[channels*(y*width+x)+2]
#define ALPHA_CHANNEL(img,width,x,y,channels)  img[channels*(y*width+x)+3]
#define CLAMP(ll,x,ul) (((x)<(ll)) ? (ll) : (((x)>(ul)) ? (ul) : (x)))

	static int unscramble[4] = { 2, 3, 1, 0 };
	static int compressParams[16][4] = { { -8, -2,  2, 8 },{ -8, -2,  2, 8 },{ -17, -5, 5, 17 },{ -17, -5, 5, 17 },{ -29, -9, 9, 29 },{ -29, -9, 9, 29 },{ -42, -13, 13, 42 },{ -42, -13, 13, 42 },{ -60, -18, 18, 60 },{ -60, -18, 18, 60 },{ -80, -24, 24, 80 },{ -80, -24, 24, 80 },{ -106, -33, 33, 106 },{ -106, -33, 33, 106 },{ -183, -47, 47, 183 },{ -183, -47, 47, 183 } };

	// Decompress an ETC1 block (or ETC2 using individual or differential mode).
	// NO WARRANTY --- SEE STATEMENT IN TOP OF FILE (C) Ericsson AB 2005-2013. All Rights Reserved.
	void decompressBlockDiffFlipC(unsigned int block_part1, unsigned int block_part2, uint8 *img, int width, int height, int startx, int starty, int channels)
	{

		UNREFERENCED_PARAMETER(height);
		uint8 avg_color[3], enc_color1[3], enc_color2[3];
		signed char diff[3];
		int table;
		int index, shift;
		int r, g, b;
		int diffbit;
		int flipbit;

		diffbit = (GETBITSHIGH(block_part1, 1, 33));
		flipbit = (GETBITSHIGH(block_part1, 1, 32));

		if (!diffbit)
		{
			// We have diffbit = 0.

			// First decode left part of block.
			avg_color[0] = GETBITSHIGH(block_part1, 4, 63);
			avg_color[1] = GETBITSHIGH(block_part1, 4, 55);
			avg_color[2] = GETBITSHIGH(block_part1, 4, 47);

			// Here, we should really multiply by 17 instead of 16. This can
			// be done by just copying the four lower bits to the upper ones
			// while keeping the lower bits.
			avg_color[0] |= (avg_color[0] << 4);
			avg_color[1] |= (avg_color[1] << 4);
			avg_color[2] |= (avg_color[2] << 4);

			table = GETBITSHIGH(block_part1, 3, 39) << 1;

			unsigned int pixel_indices_MSB, pixel_indices_LSB;

			pixel_indices_MSB = GETBITS(block_part2, 16, 31);
			pixel_indices_LSB = GETBITS(block_part2, 16, 15);

			if ((flipbit) == 0)
			{
				// We should not flip
				shift = 0;
				for (int x = startx; x<startx + 2; x++)
				{
					for (int y = starty; y<starty + 4; y++)
					{
						index = ((pixel_indices_MSB >> shift) & 1) << 1;
						index |= ((pixel_indices_LSB >> shift) & 1);
						shift++;
						index = unscramble[index];

						r = RED_CHANNEL(img, width, x, y, channels) = (uint8)CLAMP(0, avg_color[0] + compressParams[table][index], 255);
						g = GREEN_CHANNEL(img, width, x, y, channels) = (uint8)CLAMP(0, avg_color[1] + compressParams[table][index], 255);
						b = BLUE_CHANNEL(img, width, x, y, channels) = (uint8)CLAMP(0, avg_color[2] + compressParams[table][index], 255);
					}
				}
			}
			else
			{
				// We should flip
				shift = 0;
				for (int x = startx; x<startx + 4; x++)
				{
					for (int y = starty; y<starty + 2; y++)
					{
						index = ((pixel_indices_MSB >> shift) & 1) << 1;
						index |= ((pixel_indices_LSB >> shift) & 1);
						shift++;
						index = unscramble[index];

						r = RED_CHANNEL(img, width, x, y, channels) = (uint8)CLAMP(0, avg_color[0] + compressParams[table][index], 255);
						g = GREEN_CHANNEL(img, width, x, y, channels) = (uint8)CLAMP(0, avg_color[1] + compressParams[table][index], 255);
						b = BLUE_CHANNEL(img, width, x, y, channels) = (uint8)CLAMP(0, avg_color[2] + compressParams[table][index], 255);
					}
					shift += 2;
				}
			}

			// Now decode other part of block. 
			avg_color[0] = GETBITSHIGH(block_part1, 4, 59);
			avg_color[1] = GETBITSHIGH(block_part1, 4, 51);
			avg_color[2] = GETBITSHIGH(block_part1, 4, 43);

			// Here, we should really multiply by 17 instead of 16. This can
			// be done by just copying the four lower bits to the upper ones
			// while keeping the lower bits.
			avg_color[0] |= (avg_color[0] << 4);
			avg_color[1] |= (avg_color[1] << 4);
			avg_color[2] |= (avg_color[2] << 4);

			table = GETBITSHIGH(block_part1, 3, 36) << 1;
			pixel_indices_MSB = GETBITS(block_part2, 16, 31);
			pixel_indices_LSB = GETBITS(block_part2, 16, 15);

			if ((flipbit) == 0)
			{
				// We should not flip
				shift = 8;
				for (int x = startx + 2; x<startx + 4; x++)
				{
					for (int y = starty; y<starty + 4; y++)
					{
						index = ((pixel_indices_MSB >> shift) & 1) << 1;
						index |= ((pixel_indices_LSB >> shift) & 1);
						shift++;
						index = unscramble[index];

						r = RED_CHANNEL(img, width, x, y, channels) = (uint8)CLAMP(0, avg_color[0] + compressParams[table][index], 255);
						g = GREEN_CHANNEL(img, width, x, y, channels) = (uint8)CLAMP(0, avg_color[1] + compressParams[table][index], 255);
						b = BLUE_CHANNEL(img, width, x, y, channels) = (uint8)CLAMP(0, avg_color[2] + compressParams[table][index], 255);
					}
				}
			}
			else
			{
				// We should flip
				shift = 2;
				for (int x = startx; x<startx + 4; x++)
				{
					for (int y = starty + 2; y<starty + 4; y++)
					{
						index = ((pixel_indices_MSB >> shift) & 1) << 1;
						index |= ((pixel_indices_LSB >> shift) & 1);
						shift++;
						index = unscramble[index];

						r = RED_CHANNEL(img, width, x, y, channels) = (uint8)CLAMP(0, avg_color[0] + compressParams[table][index], 255);
						g = GREEN_CHANNEL(img, width, x, y, channels) = (uint8)CLAMP(0, avg_color[1] + compressParams[table][index], 255);
						b = BLUE_CHANNEL(img, width, x, y, channels) = (uint8)CLAMP(0, avg_color[2] + compressParams[table][index], 255);
					}
					shift += 2;
				}
			}
		}
		else
		{
			// We have diffbit = 1. 

			//      63 62 61 60 59 58 57 56 55 54 53 52 51 50 49 48 47 46 45 44 43 42 41 40 39 38 37 36 35 34  33  32 
			//      ---------------------------------------------------------------------------------------------------
			//     | base col1    | dcol 2 | base col1    | dcol 2 | base col 1   | dcol 2 | table  | table  |diff|flip|
			//     | R1' (5 bits) | dR2    | G1' (5 bits) | dG2    | B1' (5 bits) | dB2    | cw 1   | cw 2   |bit |bit |
			//      ---------------------------------------------------------------------------------------------------
			// 
			// 
			//     c) bit layout in bits 31 through 0 (in both cases)
			// 
			//      31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3   2   1  0
			//      --------------------------------------------------------------------------------------------------
			//     |       most significant pixel index bits       |         least significant pixel index bits       |  
			//     | p| o| n| m| l| k| j| i| h| g| f| e| d| c| b| a| p| o| n| m| l| k| j| i| h| g| f| e| d| c | b | a |
			//      --------------------------------------------------------------------------------------------------      

			// First decode left part of block.
			enc_color1[0] = GETBITSHIGH(block_part1, 5, 63);
			enc_color1[1] = GETBITSHIGH(block_part1, 5, 55);
			enc_color1[2] = GETBITSHIGH(block_part1, 5, 47);

			// Expand from 5 to 8 bits
			avg_color[0] = (enc_color1[0] << 3) | (enc_color1[0] >> 2);
			avg_color[1] = (enc_color1[1] << 3) | (enc_color1[1] >> 2);
			avg_color[2] = (enc_color1[2] << 3) | (enc_color1[2] >> 2);

			table = GETBITSHIGH(block_part1, 3, 39) << 1;

			unsigned int pixel_indices_MSB, pixel_indices_LSB;

			pixel_indices_MSB = GETBITS(block_part2, 16, 31);
			pixel_indices_LSB = GETBITS(block_part2, 16, 15);

			if ((flipbit) == 0)
			{
				// We should not flip
				shift = 0;
				for (int x = startx; x<startx + 2; x++)
				{
					for (int y = starty; y<starty + 4; y++)
					{
						index = ((pixel_indices_MSB >> shift) & 1) << 1;
						index |= ((pixel_indices_LSB >> shift) & 1);
						shift++;
						index = unscramble[index];

						r = RED_CHANNEL(img, width, x, y, channels) = (uint8)CLAMP(0, avg_color[0] + compressParams[table][index], 255);
						g = GREEN_CHANNEL(img, width, x, y, channels) = (uint8)CLAMP(0, avg_color[1] + compressParams[table][index], 255);
						b = BLUE_CHANNEL(img, width, x, y, channels) = (uint8)CLAMP(0, avg_color[2] + compressParams[table][index], 255);
					}
				}
			}
			else
			{
				// We should flip
				shift = 0;
				for (int x = startx; x<startx + 4; x++)
				{
					for (int y = starty; y<starty + 2; y++)
					{
						index = ((pixel_indices_MSB >> shift) & 1) << 1;
						index |= ((pixel_indices_LSB >> shift) & 1);
						shift++;
						index = unscramble[index];

						r = RED_CHANNEL(img, width, x, y, channels) = (uint8)CLAMP(0, avg_color[0] + compressParams[table][index], 255);
						g = GREEN_CHANNEL(img, width, x, y, channels) = (uint8)CLAMP(0, avg_color[1] + compressParams[table][index], 255);
						b = BLUE_CHANNEL(img, width, x, y, channels) = (uint8)CLAMP(0, avg_color[2] + compressParams[table][index], 255);
					}
					shift += 2;
				}
			}

			// Now decode right part of block. 
			diff[0] = GETBITSHIGH(block_part1, 3, 58);
			diff[1] = GETBITSHIGH(block_part1, 3, 50);
			diff[2] = GETBITSHIGH(block_part1, 3, 42);

			// Extend sign bit to entire byte. 
			diff[0] = (diff[0] << 5);
			diff[1] = (diff[1] << 5);
			diff[2] = (diff[2] << 5);
			diff[0] = diff[0] >> 5;
			diff[1] = diff[1] >> 5;
			diff[2] = diff[2] >> 5;

			//  Calculale second color
			enc_color2[0] = enc_color1[0] + diff[0];
			enc_color2[1] = enc_color1[1] + diff[1];
			enc_color2[2] = enc_color1[2] + diff[2];

			// Expand from 5 to 8 bits
			avg_color[0] = (enc_color2[0] << 3) | (enc_color2[0] >> 2);
			avg_color[1] = (enc_color2[1] << 3) | (enc_color2[1] >> 2);
			avg_color[2] = (enc_color2[2] << 3) | (enc_color2[2] >> 2);

			table = GETBITSHIGH(block_part1, 3, 36) << 1;
			pixel_indices_MSB = GETBITS(block_part2, 16, 31);
			pixel_indices_LSB = GETBITS(block_part2, 16, 15);

			if ((flipbit) == 0)
			{
				// We should not flip
				shift = 8;
				for (int x = startx + 2; x<startx + 4; x++)
				{
					for (int y = starty; y<starty + 4; y++)
					{
						index = ((pixel_indices_MSB >> shift) & 1) << 1;
						index |= ((pixel_indices_LSB >> shift) & 1);
						shift++;
						index = unscramble[index];

						r = RED_CHANNEL(img, width, x, y, channels) = (uint8)CLAMP(0, avg_color[0] + compressParams[table][index], 255);
						g = GREEN_CHANNEL(img, width, x, y, channels) = (uint8)CLAMP(0, avg_color[1] + compressParams[table][index], 255);
						b = BLUE_CHANNEL(img, width, x, y, channels) = (uint8)CLAMP(0, avg_color[2] + compressParams[table][index], 255);
					}
				}
			}
			else
			{
				// We should flip
				shift = 2;
				for (int x = startx; x<startx + 4; x++)
				{
					for (int y = starty + 2; y<starty + 4; y++)
					{
						index = ((pixel_indices_MSB >> shift) & 1) << 1;
						index |= ((pixel_indices_LSB >> shift) & 1);
						shift++;
						index = unscramble[index];

						r = RED_CHANNEL(img, width, x, y, channels) = (uint8)CLAMP(0, avg_color[0] + compressParams[table][index], 255);
						g = GREEN_CHANNEL(img, width, x, y, channels) = (uint8)CLAMP(0, avg_color[1] + compressParams[table][index], 255);
						b = BLUE_CHANNEL(img, width, x, y, channels) = (uint8)CLAMP(0, avg_color[2] + compressParams[table][index], 255);
					}
					shift += 2;
				}
			}
		}
	}

	void decompressBlockDiffFlip(unsigned int block_part1, unsigned int block_part2, uint8 *img, int width, int height, int startx, int starty)
	{
		decompressBlockDiffFlipC(block_part1, block_part2, img, width, height, startx, starty, 3);
	}
	//-------------------------------------------------------------------------
	// WRAPPER ADDED AROUND FUNCTION THAT DECODES A SINGLE 4X4 BLOCK INTO ETC
	// Note:  readCompressParams() must be called before each new image.
	//-------------------------------------------------------------------------
	void atiDecodeRGBBlockETC(
		unsigned char *pPixels,        //Color888_t(* pPixels)[ 4 ] [ 4 ],
		unsigned int compressed1,
		unsigned int compressed2
	)
	{
		decompressBlockDiffFlip(compressed1, compressed2, (uint8*)pPixels, 4, 4, 0, 0);
	}

	void CCodec_ATC::DecompressRGBBlock(CMP_BYTE rgbBlock[BLOCK_SIZE_4X4X4], CMP_DWORD compressedBlock[2])
	{
		Color888_t destRGB[4][4];

		unsigned int colorLow565or1555 = compressedBlock[0] & 0xffff;
		unsigned int colorHigh565 = (compressedBlock[0] >> 16) & 0xffff;

		atiDecodeRGBBlockATITC(&destRGB, compressedBlock[1], colorLow565or1555, colorHigh565);

		for (DWORD y = 0; y < 4; y++)
		{
			for (DWORD x = 0; x < 4; x++)
			{
				rgbBlock[(x * 16) + (y * 4) + RGBA8888_CHANNEL_R] = destRGB[x][y].red;
				rgbBlock[(x * 16) + (y * 4) + RGBA8888_CHANNEL_G] = destRGB[x][y].green;
				rgbBlock[(x * 16) + (y * 4) + RGBA8888_CHANNEL_B] = destRGB[x][y].blue;
			}
		}
	}

	void CCodec_ATC::DecompressRGBABlock_ExplicitAlpha(CMP_BYTE rgbaBlock[BLOCK_SIZE_4X4X4], CMP_DWORD compressedBlock[4])
	{
		CMP_BYTE alphaBlock[BLOCK_SIZE_4X4];

		DecompressExplicitAlphaBlock(alphaBlock, &compressedBlock[ATC_OFFSET_ALPHA]);
		DecompressRGBBlock(rgbaBlock, &compressedBlock[ATC_OFFSET_RGB]);

		for (CMP_DWORD i = 0; i < 16; i++)
			((DWORD*)rgbaBlock)[i] = (alphaBlock[i] << RGBA8888_OFFSET_A) | (((DWORD*)rgbaBlock)[i] & ~(BYTE_MASK << RGBA8888_OFFSET_A));
	}

#define EXPLICIT_ALPHA_PIXEL_MASK 0xf
#define EXPLICIT_ALPHA_PIXEL_BPP 4
	void CCodec_ATC::DecompressExplicitAlphaBlock(CMP_BYTE alphaBlock[BLOCK_SIZE_4X4], CMP_DWORD compressedBlock[2])
	{
		for (int i = 0; i<16; i++)
		{
			int nBlock = i < 8 ? 0 : 1;
			CMP_BYTE cAlpha = (CMP_BYTE)((compressedBlock[nBlock] >> ((i % 8) * EXPLICIT_ALPHA_PIXEL_BPP)) & EXPLICIT_ALPHA_PIXEL_MASK);
			alphaBlock[i] = (CMP_BYTE)((cAlpha << EXPLICIT_ALPHA_PIXEL_BPP) | cAlpha);
		}
	}	


#define SWIZZLE_DWORD(i) ((((i >> 24) & BYTE_MASK)) | (((i >> 16) & BYTE_MASK) << 8) | (((i >> 8) & BYTE_MASK) << 16) | ((i & BYTE_MASK) << 24))

	void CCodec_ETC::DecompressRGBBlock(CMP_BYTE rgbBlock[BLOCK_SIZE_4X4X4], CMP_DWORD compressedBlock[2])
	{
		Color888_t destRGB[4][4];

		unsigned int uiCompressedBlockHi = SWIZZLE_DWORD(compressedBlock[0]);
		unsigned int uiCompressedBlockLo = SWIZZLE_DWORD(compressedBlock[1]);

		atiDecodeRGBBlockETC((unsigned char *)&destRGB, uiCompressedBlockHi, uiCompressedBlockLo);

		for (DWORD y = 0; y < 4; y++)
		{
			for (DWORD x = 0; x < 4; x++)
			{
				rgbBlock[(x * 16) + (y * 4) + RGBA8888_CHANNEL_R] = destRGB[x][y].red;
				rgbBlock[(x * 16) + (y * 4) + RGBA8888_CHANNEL_G] = destRGB[x][y].green;
				rgbBlock[(x * 16) + (y * 4) + RGBA8888_CHANNEL_B] = destRGB[x][y].blue;
				rgbBlock[(x * 16) + (y * 4) + RGBA8888_CHANNEL_A] = 0xFF;
			}
		}
	}


	void CCodec_ETC::DecompressRGBABlock_ExplicitAlpha(CMP_BYTE rgbaBlock[BLOCK_SIZE_4X4X4], CMP_DWORD compressedBlock[4])
	{
		CMP_BYTE alphaBlock[BLOCK_SIZE_4X4];

		DecompressExplicitAlphaBlock(alphaBlock, &compressedBlock[ATC_OFFSET_ALPHA]);
		DecompressRGBBlock(rgbaBlock, &compressedBlock[ATC_OFFSET_RGB]);

		for (CMP_DWORD i = 0; i < 16; i++)
			((DWORD*)rgbaBlock)[i] = (alphaBlock[i] << RGBA8888_OFFSET_A) | (((DWORD*)rgbaBlock)[i] & ~(BYTE_MASK << RGBA8888_OFFSET_A));
	}

	void CCodec_ETC::DecompressExplicitAlphaBlock(CMP_BYTE alphaBlock[BLOCK_SIZE_4X4], CMP_DWORD compressedBlock[2])
	{
		for (int i = 0; i<16; i++)
		{
			int nBlock = i < 8 ? 0 : 1;
			CMP_BYTE cAlpha = (CMP_BYTE)((compressedBlock[nBlock] >> ((i % 8) * EXPLICIT_ALPHA_PIXEL_BPP)) & EXPLICIT_ALPHA_PIXEL_MASK);
			alphaBlock[i] = (CMP_BYTE)((cAlpha << EXPLICIT_ALPHA_PIXEL_BPP) | cAlpha);
		}
	}
}