#include "Texture.h"

Texture::Texture()
{
	mTexture = NULL;
	mHeight = 0;
	mWidth = 0;
	mTotalPixels = 0;
	mPitch = 0;
	mPixels = NULL;

	r = 0xFF; 
	g = 0xFF;
	b = 0xFF;
}

Texture::~Texture()
{
}

bool Texture::loadFromFile(std::string path, SDL_Renderer *renderer)
{
	SDL_Texture* newTexture = NULL;

	// Load image from the specified path passed in parameters
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());


	if (loadedSurface != NULL)
	{
		// SDL_TEXTUREACCESS_STREAMING
		// changes frequently, lockable
		newTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, loadedSurface->w, loadedSurface->h);
	
		//Lock texture for manipulation
		SDL_LockTexture(newTexture, NULL, &mPixels, &mPitch);
		
		// Copy loaded/formatted surface pixels
		memcpy(mPixels, loadedSurface->pixels, loadedSurface->pitch * loadedSurface->h);
		
		// Unlock texture to update
		SDL_UnlockTexture(newTexture);
		if (newTexture != NULL)
		{
			// Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;

			// set other important values
			mPitch = loadedSurface->pitch;
			mClip = loadedSurface->clip_rect;
			mFormat = *loadedSurface->format;

			// REMOVED - mPixels = loadedSurface->pixels; 
			// Would set before instantiated and cause potential crashes
		}

		SDL_FreeSurface(loadedSurface);
	}

	mTexture = newTexture;

	return mTexture;
}

SDL_Texture* Texture::getTexture()
{
	return mTexture;
}

int Texture::getWidth()
{
	return mWidth;
}

int Texture::getHeight()
{
	return mHeight;
}

void* Texture::getPixels()
{
	return mPixels;
}

int Texture::getTotalPixels()
{
	mTotalPixels = getHeight() * getWidth();
	return mTotalPixels;
}

int Texture::getPitch()
{
	return mPitch;
}

SDL_Rect Texture::getRect()
{
	return mClip;
}

SDL_PixelFormat Texture::getFormat()
{
	return mFormat;
}

void Texture::setColour(Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void Texture::setBlendMode(SDL_BlendMode blend)
{

}
