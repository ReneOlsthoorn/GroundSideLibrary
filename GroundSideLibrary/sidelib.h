#pragma once
#define _NO_CRT_STDIO_INLINE
//#include <stdio.h>
//#include <math.h>
//#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
//#include <windows.h>

#include "msvcrt.h"
#include "typedef_structs.h"
#include "lodepng.h"
#include "math.h"


#define SIDELIB_API __declspec(dllexport)
extern "C" SIDELIB_API int sidelib_Test();
extern "C" SIDELIB_API BYTE* sidelib_LoadImage(const char* fullPathToImage);
extern "C" SIDELIB_API BYTE* sidelib_LoadAndExpandImage(const char* fullPathToImage, int expandNr);
extern "C" SIDELIB_API void sidelib_FreeImage(BYTE * image);
extern "C" SIDELIB_API void sidelib_ConvertFonts(Uint32* source, BYTE* fontchar256, Uint16* fontchar32);
extern "C" SIDELIB_API void sidelib_ConvertFont1024(Uint32* source, Uint32* fontchar1024, int nrRows);
extern "C" SIDELIB_API void sidelib_FlipRedAndGreenInImage(BYTE * image, int SizeX, int SizeY);
extern "C" SIDELIB_API int sidelib_ApproxDistance(int dx, int dy);
extern "C" SIDELIB_API float sidelib_ApproxDistanceFloat(float dx, float dy);
extern "C" SIDELIB_API double sidelib_ApproxDivide(double y, double x);
extern "C" SIDELIB_API float sidelib_InvertFast(float x);
