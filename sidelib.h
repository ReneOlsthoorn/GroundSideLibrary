#include "typedef_structs.h"

#define SIDELIB_API __declspec(dllexport)
SIDELIB_API int sidelib_Test();
SIDELIB_API BYTE* sidelib_LoadImage(const char* fullPathToImage);
SIDELIB_API BYTE* sidelib_LoadAndExpandImage(const char* fullPathToImage, int expandNr);
SIDELIB_API void sidelib_FreeImage(BYTE * image);
SIDELIB_API void sidelib_ConvertFonts(Uint32* source, BYTE* fontchar256, Uint16* fontchar32);
SIDELIB_API void sidelib_ConvertFont1024(Uint32* source, Uint32* fontchar1024, int nrRows);
SIDELIB_API void sidelib_FlipRedAndGreenInImage(BYTE * image, int SizeX, int SizeY);
SIDELIB_API int sidelib_ApproxDistance(int dx, int dy);
SIDELIB_API float sidelib_ApproxDistanceFloat(float dx, float dy);
SIDELIB_API double sidelib_ApproxDivide(double y, double x);
SIDELIB_API float sidelib_InvertFast(float x);
