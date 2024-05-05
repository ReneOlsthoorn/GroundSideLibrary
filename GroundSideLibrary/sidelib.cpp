#include "sidelib.h"

SIDELIB_API int TestFunction() {
	return 42;
}

BYTE* GC_LoadImage(const char* fullPathToImage) {
	BYTE* image = NULL;
	unsigned int width, height;
	lodepng_decode32_file(&image, &width, &height, fullPathToImage);
	return image;
}


void GC_FreeImage(BYTE* image) {
	free(image);
}
