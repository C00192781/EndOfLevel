#pragma once

#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>


class Texture
{
public:
	Texture();
	~Texture();

	bool loadFromFile(std::string path, SDL_Renderer *renderer);
	SDL_Texture* getTexture();
	int getWidth();
	int getHeight();
	void* getPixels();
	int getTotalPixels();
	int getPitch();
	SDL_Rect getRect();
	SDL_PixelFormat getFormat();
	
	void setColour(Uint8 red, Uint8 green, Uint8 blue);
	void setBlendMode(SDL_BlendMode blend);

private:
	SDL_Texture* mTexture;

	// image dimensions
	int mWidth;
	int mHeight;
	int mPitch;
	void* mPixels;
	int mTotalPixels;
	// added so I can update texture
	SDL_Rect mClip;
	SDL_PixelFormat mFormat;

	int r, g, b; 
};