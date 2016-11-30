// etcpack_lib.h
#ifndef H_TEX_ETCPACK_LIB_H
#define H_TEX_ETCPACK_LIB_H

#define ETCPACK_VERSION_S "2.74"

// data types
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef short int16;

void decompressBlockETC2c(unsigned int block_part1, unsigned int block_part2, uint8 *img, int width, int height, int startx, int starty, int channels);
void decompressBlockETC2(unsigned int block_part1, unsigned int block_part2, uint8 *img, int width, int height, int startx, int starty);
void read_big_endian_2byte_word(unsigned short *blockadr, FILE *f);
void read_big_endian_4byte_word(unsigned int *blockadr, FILE *f);
void unstuff57bits(unsigned int planar_word1, unsigned int planar_word2, unsigned int &planar57_word1, unsigned int &planar57_word2);
void unstuff59bits(unsigned int thumbT_word1, unsigned int thumbT_word2, unsigned int &thumbT59_word1, unsigned int &thumbT59_word2);
void unstuff58bits(unsigned int thumbH_word1, unsigned int thumbH_word2, unsigned int &thumbH58_word1, unsigned int &thumbH58_word2);
void decompressColor(int R_B, int G_B, int B_B, uint8(colors_RGB444)[2][3], uint8(colors)[2][3]);
void calculatePaintColors59T(uint8 d, uint8 p, uint8(colors)[2][3], uint8(possible_colors)[4][3]);
void calculatePaintColors58H(uint8 d, uint8 p, uint8(colors)[2][3], uint8(possible_colors)[4][3]);
void decompressBlockTHUMB59T(unsigned int block_part1, unsigned int block_part2, uint8 *img, int width, int height, int startx, int starty);
void decompressBlockTHUMB58H(unsigned int block_part1, unsigned int block_part2, uint8 *img, int width, int height, int startx, int starty);
void decompressBlockPlanar57(unsigned int compressed57_1, unsigned int compressed57_2, uint8 *img, int width, int height, int startx, int starty);
void decompressBlockDiffFlip(unsigned int block_part1, unsigned int block_part2, uint8 *img, int width, int height, int startx, int starty);
void decompressBlockDifferentialWithAlpha(unsigned int block_part1, unsigned int block_part2, uint8* img, uint8* alpha, int width, int height, int startx, int starty);
void decompressBlockETC21BitAlpha(unsigned int block_part1, unsigned int block_part2, uint8 *img, uint8* alphaimg, int width, int height, int startx, int starty);
void decompressBlockTHUMB58HAlpha(unsigned int block_part1, unsigned int block_part2, uint8 *img, uint8* alpha, int width, int height, int startx, int starty);
void decompressBlockTHUMB59TAlpha(unsigned int block_part1, unsigned int block_part2, uint8 *img, uint8* alpha, int width, int height, int startx, int starty);
uint8 getbit(uint8 input, int frompos, int topos);
int clamp(int val);
void decompressBlockAlphaC(uint8* data, uint8* img, int width, int height, int ix, int iy, int channels);
void decompressBlockAlpha(uint8* data, uint8* img, int width, int height, int ix, int iy);
uint16 get16bits11bits(int base, int table, int mul, int index);
void decompressBlockAlpha16bit(uint8* data, uint8* img, int width, int height, int ix, int iy);
int16 get16bits11signed(int base, int table, int mul, int index);
void setupAlphaTable();
#endif