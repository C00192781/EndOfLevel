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
	// STANDARD DEVIATION
	// ignoring a Gaussian function beyond a radius of 3�sigma 
	// still leaves you with more than 97% of its total information
	double sigma = kernelRadius / 3.0f;
	double sigmaSquared = pow(sigma, 2.0f);
	double radiusSquared = pow(kernelRadius, 2.0f);
	double sqrt1 = 1.0f / (2.0f * radiusSquared);
	double sqrt2 = 1.0f / (sqrt(2.0f * PI) * kernelRadius);

	float kernelSize = kernelRadius * 2 + 1;

	float * tempKernel = nullptr;
	tempKernel = new float[kernelSize];

	// accumulate values
	double radiusMod = 1.0f;
	int r = -kernelRadius;
	for (int index = 0; index < kernelSize; index++)
	{
		double x = r * radiusMod;
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
	//SDL_LockSurface(surface);
	Uint8 r, g, b, a = 0;
		
	// NEEDS MPIXELS
	Uint32* tempPixelArray = NULL;
	tempPixelArray = (Uint32*)texture->getPixels();

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
	
	// x pass
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

	// y pass
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


	//memcpy(mPixels, loadedSurface->pixels, loadedSurface->pitch * loadedSurface->h);
	return pixelArray;
	//SDL_UnlockSurface(surface);
}

Uint32 * Bloom::BrightPass(Texture * texture, SDL_Surface * surface)
{
	
	Uint8 r, g, b, a = 0;
	int w = texture->getWidth();
	int h = texture->getHeight();

	float luminance[] = { 0.2126f, 0.7152f, 0.0722f };


	int totalPixels = texture->getTotalPixels();
	Uint32* tempPixelArray = nullptr;
	tempPixelArray = (Uint32 *)texture->getPixels();

	
	for (int x = 0; x < w; x++)
	{
		for (int y = 0; y < h; y++)
		{
			r = tempPixelArray[y * w + x] >> 16;
			g = tempPixelArray[y * w + x] >> 8;
			b = tempPixelArray[y * w + x] >> 0;
			a = tempPixelArray[y * w + x] >> 24;

			float rTest = r;
			float gTest = g;
			float bTest = b;
		
			float rTestMul = r * luminance[0];
			float gTestMul = g * luminance[1];
			float bTestMul = b * luminance[2];

			float BRIGHT_PASS_THRESHOLD = 50;
			// 50 is placeholder - user will input, e.g. 10, 20, 30
			if ((rTestMul + gTestMul + bTestMul) > BRIGHT_PASS_THRESHOLD)
			{
				float ratio = 1.0 / max(max(rTest, gTest), bTest);

				r += r *ratio;
				g += g *ratio;
				b += b *ratio;
			}
			else
			{
				r = 0;
				g = 0;
				b = 0;
			}

			Uint32 RGBA;
			RGBA = (r << 16) | (g << 8) | (b << 0) | (a << 24);
			tempPixelArray[y * w + x] = RGBA;
		}	
	}		

	return tempPixelArray;
}

Uint32 * Bloom::BloomEffect(Texture * texture, SDL_Surface * surface, int totalPixels)
{
	Uint8 r, g, b, a = 0;
	int w = texture->getWidth();
	int h = texture->getHeight();

	// NEEDS MPIXELS
	Uint32* tempPixelArray = NULL;
	tempPixelArray = (Uint32*)texture->getPixels();

	Uint32* pixelArray = nullptr;
	pixelArray = new Uint32[totalPixels];

	for (int x = 0; x < w; x++)
	{
		for (int y = 0; y < h; y++)
		{
			r = tempPixelArray[y * w + x] >> 16;
			g = tempPixelArray[y * w + x] >> 8;
			b = tempPixelArray[y * w + x] >> 0;
			a = tempPixelArray[y * w + x] >> 24;

			const float BLEND = 0.3;
		
			r += r * BLEND;
			g += g * BLEND;
			b += b * BLEND;
			a += a * BLEND;


			Uint32 RGBA;
			RGBA = (r << 16) | (g << 8) | (b << 0) | (a << 24);
			pixelArray[y * w + x] = RGBA;
		}

	}

	return pixelArray;
}



