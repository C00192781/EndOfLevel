#include "Growing.h"

Growing::Growing()
{
	surfaceW = 0;
	surfaceH = 0;
	surfaceX = 0;
	surfaceY = 0;
}

Growing::~Growing()
{

}

void Growing::Animation(Texture* texture, SDL_Surface* screenSurface, SDL_Surface* stretch, SDL_Rect* rect, float x, float y, float startWidth, float startHeight, float endWidth, float endHeight, int timerStart, int timerIntermediate, int timerEnd)
{
	surfaceW = startWidth;
	surfaceH = startHeight;
	surfaceX = x;
	surfaceY = y;

	//std::cout << "startwidth" << x << std::endl;

	if (surfaceW < startWidth)
	{
		surfaceW += 1;
	}
	if (surfaceH < endHeight)
	{
		surfaceH += 1;
	}

	std::cout << "startwidth" << surfaceH << std::endl;

	rect->x = surfaceX;
	rect->y = surfaceY;
	rect->w = surfaceW;
	rect->h = surfaceH;
	//SDL_BlitScaled(stretchedSurface, &myTexture.getRect(), screen, &stretchedRect);
	SDL_BlitScaled(stretch, &texture->getRect(), screenSurface, rect);




}
