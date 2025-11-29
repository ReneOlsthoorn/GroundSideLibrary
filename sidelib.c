#include <windows.h>
#include "lodepng.h"
#include "sidelib.h"
#include <math.h>


/*
  PI = 3.14159265358979323846, maar dat niet door een float exact benaderd worden.
  Zelfs al probeer je 3.14159265, dan krijg je nog 3.14159274.
  Door de compiler wordt zelf het meest juiste getal voor het datatype bepaald. We kunnen dus prima het veel te scherpe M_PI gebruiken, want de compiler schaalt het zelf terug.
*/
const float PI = 3.14159265358979323846;          // 3.14
const float TWO_PI = 2 * PI;  // 6.28
const float HALF_PI = PI / 2.0;   // 1.57



SIDELIB_API int sidelib_Test() {
	return 42;
}


SIDELIB_API BYTE* sidelib_LoadImage(const char* fullPathToImage) {
	BYTE* image = NULL;
	unsigned int width, height;
	lodepng_decode32_file(&image, &width, &height, fullPathToImage);
	return image;
}


BYTE* sidelib_LoadAndExpandImage(const char* fullPathToImage, int expandNr) {
	BYTE* image = NULL;
	unsigned int width, height;
	lodepng_decode32_file(&image, &width, &height, fullPathToImage);
	Uint32* imageSrc = (Uint32*)image;

	const int pixelSize = 4;
	int newSize = width * height * expandNr * expandNr * pixelSize;

	Uint32* newImage = (Uint32*)malloc(newSize);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int j = 0; j < expandNr; j++) {
				for (int i = 0; i < expandNr; i++) {
					newImage[ (((y*expandNr)+j)*(width*expandNr)) + ((x*expandNr)+i) ] = imageSrc[(y*width) + x];
				}
			}
		}
	}
	return (BYTE*)newImage;
}


SIDELIB_API void sidelib_FreeImage(BYTE* image) {
	free(image);
}


SIDELIB_API void sidelib_ConvertFonts(Uint32* source, BYTE* fontchar256, Uint16* fontchar32) {
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


SIDELIB_API void sidelib_ConvertFont1024(Uint32* source, Uint32* fontchar1024, int nrRows) {
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


SIDELIB_API void sidelib_FlipRedAndGreenInImage(BYTE* image, int SizeX, int SizeY)
{
	for (int i = 0; i < SizeX * SizeY; i++) {
		int tmp = image[(i * 4) + 2];
		image[(i * 4) + 2] = image[(i * 4)];
		image[(i * 4)] = tmp;
	}
}


SIDELIB_API int sidelib_ApproxDistance(int dx, int dy)
{
	int min, max, approx;

	if (dx < 0) dx = -dx;
	if (dy < 0) dy = -dy;

	if (dx < dy) {
		min = dx;
		max = dy;
	}
	else {
		min = dy;
		max = dx;
	}
	approx = (max * 1007) + (min * 441);
	if (max < (min << 4)) {
		approx -= (max * 40);
	}
	return ((approx + 512) >> 10);  // add 512 for proper rounding
}


SIDELIB_API float sidelib_ApproxDistanceFloat(float dx, float dy)
{
	// https://gamedev.stackexchange.com/questions/69241/how-to-optimize-the-distance-function
	float min, max, approx;
	const float dxFragment = (1007.0f / 1024.0f);
	const float dyFragment = (441.0f / 1024.0f);

	if (dx < 0) dx = -dx;
	if (dy < 0) dy = -dy;

	if (dx < dy)
	{
		min = dx;
		max = dy;
	}
	else {
		min = dy;
		max = dx;
	}
	return (dxFragment * dx) + (dyFragment * dy);
}


SIDELIB_API double sidelib_ApproxDivide(double y, double x) {
	// https://stackoverflow.com/questions/31031223/fast-approximate-float-division
	// calculates y/x
	union {
		double dbl;
		unsigned long long ull;
	} u;
	u.dbl = x;                      // x = x
	u.ull = (0xbfcdd6a18f6a6f52ULL - u.ull) >> (unsigned char)1;
	// pow( x, -0.5 )
	u.dbl *= u.dbl;                 // pow( pow(x,-0.5), 2 ) = pow( x, -1 ) = 1.0/x
	return u.dbl * y;               // (1.0/x) * y = y/x
}


SIDELIB_API float sidelib_InvertFast(float x) {
	union { float f; int i; } v;
	float w, sx;
	int m;

	sx = (x < 0) ? -1 : 1;
	x = sx * x;

	v.i = (int)(0x7EF127EA - *(uint32_t*)&x);
	w = x * v.f;

	// Efficient Iterative Approximation Improvement in horner polynomial form.
	v.f = v.f * (2 - w);     // Single iteration, Err = -3.36e-3 * 2^(-flr(log2(x)))
	// v.f = v.f * ( 4 + w * (-6 + w * (4 - w)));  // Second iteration, Err = -1.13e-5 * 2^(-flr(log2(x)))
	// v.f = v.f * (8 + w * (-28 + w * (56 + w * (-70 + w *(56 + w * (-28 + w * (8 - w)))))));  // Third Iteration, Err = +-6.8e-8 *  2^(-flr(log2(x)))

	return v.f * sx;
}


float sidelib_Atan2Approx(float y, float x)
{
	const float n1 = 0.97239411f;
	const float n2 = -0.19194795f;
	float result = 0.0f;
	if (x != 0.0f)
	{
		const union { float flVal; Uint32 nVal; } tYSign = { y };
		const union { float flVal; Uint32 nVal; } tXSign = { x };
		if (fabsf(x) >= fabsf(y))
		{
			union { float flVal; Uint32 nVal; } tOffset = { PI };
			// Add or subtract PI based on y's sign.
			tOffset.nVal |= tYSign.nVal & 0x80000000u;
			// No offset if x is positive, so multiply by 0 or based on x's sign.
			tOffset.nVal *= tXSign.nVal >> 31;
			result = tOffset.flVal;
			const float z = y / x;
			result += (n1 + n2 * z * z) * z;
		}
		else // Use atan(y/x) = pi/2 - atan(x/y) if |y/x| > 1.
		{
			union { float flVal; Uint32 nVal; } tOffset = { HALF_PI };
			// Add or subtract PI/2 based on y's sign.
			tOffset.nVal |= tYSign.nVal & 0x80000000u;
			result = tOffset.flVal;
			const float z = x / y;
			result -= (n1 + n2 * z * z) * z;
		}
	}
	else if (y > 0.0f)
	{
		result = HALF_PI;
	}
	else if (y < 0.0f)
	{
		result = -HALF_PI;
	}
	return result;
}


float sidelib_SinusApprox1(float x) {
	double sin;

	//always wrap input angle to -PI..PI
	if (x < -PI) {
		while (x < -PI) {
			x += TWO_PI;
		}
	}
	else if (x > PI) {
		while (x > PI) {
			x -= TWO_PI;
		}
	}

	if (x < 0)
	{
		sin = 1.27323954 * x + 0.405284735 * x * x;

		if (sin < 0)
			sin = .225 * (sin * -sin - sin) + sin;
		else
			sin = .225 * (sin * sin - sin) + sin;
	}
	else
	{
		sin = 1.27323954 * x - 0.405284735 * x * x;

		if (sin < 0)
			sin = .225 * (sin * -sin - sin) + sin;
		else
			sin = .225 * (sin * sin - sin) + sin;
	}

	return (float)sin;
}

/*
float sidelib_SinusApprox2(float x)
{
	const float PI = (float)PI;
	const float B = 4 / PI;
	const float C = -4 / (PI * PI);

	while (x > (TWO_PI)) { x -= TWO_PI; }
	while (x < 0.0f) { x += TWO_PI; }

	x = PI - x;

	float y = (B * x) + (C * x * (float)fabs(x));

	// -- begin block can be removed for less precision

	//  const float Q = 0.775;
	const float P = 0.225f;
	y = P * (y * (float)fabs(y) - y) + y;   // Q * y + P * y * abs(y)

	// -- end block

	return y;
}
*/

float sidelib_SinusApprox3(float x) {
	// https://stackoverflow.com/questions/23837916/a-faster-but-less-accurate-fsin-for-intel-asm

	//always wrap input angle to -PI..PI
	if (x < -PI) {
		while (x < -PI) {
			x += TWO_PI;
		}
	}
	else if (x > PI) {
		while (x > PI) {
			x -= TWO_PI;
		}
	}

	const float xx = x * x;
	const float s = x + (x * xx) * (-0.16612511580269618f + xx * (8.0394356072977748e-3f + xx * -1.49414020045938777495e-4f));

	return s;
}

int sidelib_RadiansToDegree(float radians) {
	return (radians * 180.0f) / PI;
}


int sidelib_msys_rand(int* seed)
{
	seed[0] = seed[0] * 0x343FD + 0x269EC3;
	return((seed[0] >> 16) & 32767);
}


float sidelib_msys_frand(int* seed)
{
	seed[0] = seed[0] * 0x343FD + 0x269EC3;
	//seed[0] *= 16807;

	const unsigned int a = (((unsigned int)seed[0]) >> 9) | 0x3f800000;
	const float res = (*((float*)&a)) - 1.0f;

	return(res);
}


float sidelib_msys_sfrand(int* seed)
{
	seed[0] = seed[0] * 0x343FD + 0x269EC3;
	//seed[0] *= 16807;

	const unsigned int a = (((unsigned int)seed[0]) >> 9) | 0x40000000;
	const float res = (*((float*)&a)) - 3.0f;

	return(res);
}

float sidelib_SinusApprox(float x) {
	return sidelib_SinusApprox3(x);
}

float sidelib_CosinusApprox(float x) {
	return sidelib_SinusApprox3(x + HALF_PI);
}

