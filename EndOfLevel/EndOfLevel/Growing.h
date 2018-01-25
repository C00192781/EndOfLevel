#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "Texture.h"

class Growing
{
public:
	Growing();
	~Growing();

	void Animation(Texture* texture, SDL_Surface* screenSurface, SDL_Surface* stretch,  SDL_Rect * rect, float x, float y, float startWidth, float startHeight, float endWidth, float endHeight, int timerStart, int timerIntermediate, int timerEnd);



private:

	float surfaceX;
	float surfaceY;
	float surfaceW;
	float surfaceH;
};