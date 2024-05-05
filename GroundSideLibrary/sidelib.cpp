#include "sidelib.h"


SIDELIB_API int sidelib_Test() {
	return 42;
}


BYTE* sidelib_LoadImage(const char* fullPathToImage) {
	BYTE* image = NULL;
	unsigned int width, height;
	lodepng_decode32_file(&image, &width, &height, fullPathToImage);
	return image;
}


void sidelib_FreeImage(BYTE* image) {
	free(image);
}


void sidelib_FlipRedAndGreenInImage(BYTE* image, int SizeX, int SizeY)
{
	for (int i = 0; i < SizeX * SizeY; i++) {
		int tmp = image[(i * 4) + 2];
		image[(i * 4) + 2] = image[(i * 4)];
		image[(i * 4)] = tmp;
	}
}


void sidelib_ConvertFonts(Uint32* source, BYTE* fontchar256, Uint16* fontchar32) {
	// fontchar256: We convert to a 256x256 byte characterset, which contains 256 bytes for each character. The character is 16x16 bytes. Byte value 0 = background, value 255 = foreground.
	// fontchar32:  We convert to a 2*16 byte characterset, which contains 32 bytes for each character. The character is 2x16 bytes. The first 2 bytes contain the 16 bits of the first row.
	for (int c = 0; c < 256; c++) {
		int sourceRow = c / 16;
		int sourceCol = c % 16;
		for (int x = 0; x < 16; x++) {
			for (int y = 0; y < 16; y++) {
				Uint32 pixel = source[(sourceRow * 16 * 16 * 16) + (sourceCol * 16) + (y * 16 * 16) + x];
				int pixelIsSet = (pixel & 0x00ffffff) ? 1 : 0;
				fontchar256[(c * 16 * 16) + (y * 16) + x] = pixelIsSet ? 0xff : 0;
				fontchar32[(c * 16) + y] = fontchar32[(c * 16) + y] | (pixelIsSet << (15 - x));
			}
		}
	}
}


void sidelib_ConvertFont1024(Uint32* source, Uint32* fontchar1024, int nrRows) {
	int nrCharacters = nrRows * 16;
	const int rowSizeInDwords = 16 * 16 * 16;
	for (int c = 0; c < nrCharacters; c++) {
		int sourceRow = c / 16;
		int sourceCol = c % 16;
		for (int x = 0; x < 16; x++) {
			for (int y = 0; y < 16; y++) {
				Uint32 pixel = source[(sourceRow * rowSizeInDwords) + (sourceCol * 16) + (y * 16 * 16) + x];
				fontchar1024[(c * 16 * 16) + (y * 16) + x] = pixel;
			}
		}
	}
}

