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

	void Animation(Texture* texture, SDL_Surface* screenSurface, SDL_Surface* stretch,  SDL_Rect * rect, float *x, float *y, float *startWidth, float *startHeight, float *endWidth, float *endHeight, float timeToStart, float timeToEnd);
	bool completed();
private:
	uint32_t lastTickTime, delta;
	bool complete;
	float offsetX, offsetY;
};