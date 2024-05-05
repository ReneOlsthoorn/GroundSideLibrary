#pragma once
#define _NO_CRT_STDIO_INLINE
//#include <stdio.h>
//#include <math.h>
//#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
//#include <windows.h>

#include "msvcrt.h"
#include "typedef_structs.h"
#include "lodepng.h"


#define SIDELIB_API __declspec(dllexport)
extern "C" SIDELIB_API int sidelib_Test();
extern "C" SIDELIB_API BYTE * sidelib_LoadImage(const char* fullPathToImage);
extern "C" SIDELIB_API void sidelib_FreeImage(BYTE * image);
