#include "Bloom.h"

Bloom::Bloom()
{
	PI = 3.14159265359;
}

Bloom::~Bloom()
{

}

void Bloom::gaussianKernel(float ** kernel, int kernelRadius)
{
	double sum = 0.0f;
	double radiusSquared = pow(kernelRadius, 2.0f);
	double sqrt1 = 1.0f / (2.0f * radiusSquared);
	double sqrt2 = 1.0f / (sqrt(2.0f * PI) * kernelRadius);

	float kernelSize = kernelRadius * 2 + 1;

	float * tempKernel = nullptr;
	tempKernel = new float[kernelSize];

	// accumulate values
	int r = -kernelRadius;
	for (int index = 0; index < kernelSize; index++)
	{
		double x = r;
		x *= x;
		tempKernel[index] =
			sqrt2 * exp(-x * sqrt1);
		r++;
		sum += tempKernel[index];
	}

	// normalize
	for (int index = 0; index < kernelSize; index++)
	{
		tempKernel[index] /= sum;
	}
	*kernel = new float[kernelSize];
	*kernel = tempKernel;
}

Uint32 * Bloom::Blur(Texture *texture, int kernelRadius, int totalPixels, SDL_Surface* surface)
{
	
	Uint8 r, g, b, a = 0;
		
	// NEEDS MPIXELS
	Uint32* tempPixelArray = NULL;
	tempPixelArray = (Uint32*)texture->getPixels();
	// pass in results
	Uint32* pixelArray = nullptr;
	pixelArray = new Uint32[totalPixels];

	// need to be int as opposed to a float
	int w = texture->getWidth();
	int h = texture->getHeight();
	// pixels per row
	int kernelSize = 1 + (kernelRadius * 2);

	int clampH = h - 1;
	int clampW = w - 1;

	float * kernel = nullptr;
	gaussianKernel(&kernel, kernelRadius);
	
	// Y PASS
	for (int x = 0; x < w; x++)
	{
		for (int y = 0; y < h; y++)
		{
			Uint32 sum = 0;
			int row = y * w;
			for (int offsetX = 0; offsetX < kernelSize; offsetX++)
			{
				// it is possible for the inserted position into the array to be outside the bounds of the array
				// if this happens the program will crash
				// so we clamp the x index
				int indexX = clamp(0, clampW, (x - kernelRadius) + offsetX);

				// pixel data exists as a one dimensional array
				// so for a 10 x 10 portion of a screen - 100
				// This cannot be handled as you would a two dimensional array 

				// As this is a one dimensional array, 
				// and x, y coordinates needed to be handled
				// I need to translate these coordinates to their actual position inside this array.

				// Pixel = (y * pitch) + x;
				// Pitch/4 is interchangeable with the image's width
	
				// to access a certain pixel
				// ptr [offset + x] = colour;

				int overallPosition = indexX + row;

				// modify separate channels individually
				r = tempPixelArray[overallPosition] >> 16;
				g = tempPixelArray[overallPosition] >> 8;
				b = tempPixelArray[overallPosition] >> 0;
				a = tempPixelArray[overallPosition] >> 24; 

				
				// multiply by weights
				// must be done before rebuilding pixel colour
				a = a * kernel[offsetX];
				r = r * kernel[offsetX];
				g = g * kernel[offsetX];
				b = b * kernel[offsetX];

				// rebuild pixel colour
				Uint32 RGBA = (r << 16) | (g << 8) | (b << 0) | (a << 24);

				// sum up overall pixel colour for a row
				sum = sum + RGBA;
			}
			pixelArray[row + x] = sum;
		}
	}

	// X PASS
	for (int x = 0; x < w; x++)
	{
		for (int y = 0; y < h; y++)
		{
			Uint32 sum = 0;
			int row = y * w;
			for (int offsetY = 0; offsetY < kernelSize; offsetY++)
			{
				int indexY = clamp(0, clampH, (y - kernelRadius) + offsetY);
				int overallPosition = (indexY * w) + x;

				// modify separate channels individually
				r = tempPixelArray[overallPosition] >> 16;
				g = tempPixelArray[overallPosition] >> 8;
				b = tempPixelArray[overallPosition] >> 0;
				a = tempPixelArray[overallPosition] >> 24;


				// multiply by weights
				// must be done before rebuilding pixel colour
				a = a * kernel[offsetY];
				r = r * kernel[offsetY];
				g = g * kernel[offsetY];
				b = b * kernel[offsetY];

				// rebuild pixel colour
				Uint32 RGBA = (r << 16) | (g << 8) | (b << 0) | (a << 24);

				// sum up overall pixel colour for a row
				sum = sum + RGBA;
			}
			pixelArray[row + x] = sum;
		}
	}



	return pixelArray;

}

Uint32 * Bloom::BrightPass(Texture * texture, SDL_Surface * surface, int brightPassThreshold)
{
	
	Uint8 r, g, b, a = 0;
	Uint8 rTest, gTest, bTest, aTest = 0;
	int w = texture->getWidth();
	int h = texture->getHeight();

	// Y′ = 0.2126 R′ + 0.7152 G′ + 0.0722 B′
	float luminance[] = { 0.2126f, 0.7152f, 0.0722f };

	int totalPixels = texture->getTotalPixels();

	// requires mPixels
	Uint32* tempPixelArray = (Uint32*)texture->getPixels();
	// does not need mPixels
	Uint32* pixelArray = new Uint32[totalPixels];

	for (int x = 0; x < w; x++)
	{
		for (int y = 0; y < h; y++)
		{
			r = tempPixelArray[y * w + x] >> 16;
			g = tempPixelArray[y * w + x] >> 8;
			b = tempPixelArray[y * w + x] >> 0;
			a = tempPixelArray[y * w + x] >> 24;

			rTest = pixelArray[y * w + x] >> 16;
			gTest = pixelArray[y * w + x] >> 8;
			bTest = pixelArray[y * w + x] >> 0;
			aTest = pixelArray[y * w + x] >> 24;

			rTest = r * luminance[0];
			gTest = g * luminance[1];
			bTest = b * luminance[2];
		
			Uint8 test;
			test = (rTest << 16) | (gTest << 8) | (bTest << 0) | (a << 24);

			if (test > brightPassThreshold)
			{
				// do nothing
			}
			else
			{
				r = 0;
				g = 0;
				b = 0;
			}

			Uint32 RGBA;
			RGBA = (r << 16) | (g << 8) | (b << 0) | (a << 24);
			pixelArray[y * w + x] = RGBA;
		}	
	}		

	return pixelArray;
}

Uint32 * Bloom::ApplyBloom(Texture * texture, SDL_Surface * surface, int totalPixels, int bloomMultiplier)
{
	Uint8 r, g, b, a = 0;
	float mulR, mulG, mulB, mulA = 0;
	int w = texture->getWidth();
	int h = texture->getHeight();

	// tempPixelArray needs mPixels to get access to surface pixels
	Uint32* tempPixelArray = (Uint32*)texture->getPixels();
	
	Uint32* pixelArray = new Uint32[totalPixels];

	for (int x = 0; x < w; x++)
	{
		for (int y = 0; y < h; y++)
		{
			r = tempPixelArray[y * w + x] >> 16;
			g = tempPixelArray[y * w + x] >> 8;
			b = tempPixelArray[y * w + x] >> 0;
			a = tempPixelArray[y * w + x] >> 24;


			mulR = pixelArray[y * w + x] >> 16;
			mulG = pixelArray[y * w + x] >> 8;
			mulB = pixelArray[y * w + x] >> 0;
			mulA = pixelArray[y * w + x] >> 24;

			const float BLEND = 0.3;

			mulR = r *bloomMultiplier;
			mulG = g *bloomMultiplier;
			mulB = b *bloomMultiplier;
			mulA = a *bloomMultiplier;

			mulR += r;
			mulG += g;
			mulB += b;
			mulA += a;

			mulR = min(mulR, 255.0f);
			mulG = min(mulG, 255.0f);
			mulB = min(mulB, 255.0f);
			mulA = min(mulA, 255.0f);


			Uint32 RGBA;
			RGBA = ((Uint32)mulR << 16) | ((Uint32)mulG << 8) | ((Uint32)mulB << 0) | ((Uint32)mulA << 24);
			pixelArray[y * w + x] = RGBA;
		}

	}

	return pixelArray;
}



