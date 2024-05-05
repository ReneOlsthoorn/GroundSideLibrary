#pragma once
#define _NO_CRT_STDIO_INLINE
#include <stdio.h>
#include <math.h>
#include "typedef_structs.h"
#include "lodepng.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>

extern "C" void free(void* block);

#define SIDELIB_API __declspec(dllexport)
extern "C" SIDELIB_API int TestFunction();

extern "C" SIDELIB_API BYTE * GC_LoadImage(const char* fullPathToImage);
extern "C" SIDELIB_API void GC_FreeImage(BYTE * image);
