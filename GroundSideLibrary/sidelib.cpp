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
