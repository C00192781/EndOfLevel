#include "Growing.h"

Growing::Growing()
{
	lastTickTime, delta = 0.0f;
	complete = false;
	offsetX = 0.0f;
	offsetY = 0.0f;
}

Growing::~Growing()
{

}

void Growing::Animation(Texture* texture, SDL_Surface* screenSurface, SDL_Surface* stretch, SDL_Rect* rect, float *x, float *y, float *startWidth, float *startHeight, float *endWidth, float *endHeight, float timeToStart, float timeToEnd)
{
	if (complete == false)
	{
		uint32_t tickTime = SDL_GetTicks();
		delta = tickTime - lastTickTime;
		lastTickTime = tickTime;
		uint32_t lastTickTimeInSeconds = lastTickTime * 0.001f;

		static float mul = lastTickTime;


		SDL_FillRect(stretch, NULL, 0x000000);
		//std::cout << timeToEnd << std::endl;
		if (lastTickTimeInSeconds >= timeToStart)
		{
			static float height = *endHeight - *startHeight;
			static float width = *endWidth - *startWidth;
			static float incrementH = (height / (timeToEnd*1000));
			static float incrementW = (width / (timeToEnd*1000));
			
			if (*startHeight < *endHeight)
			{
				*startHeight += incrementH;
				offsetY += incrementH;
			}
			if (*startWidth < *endWidth)
			{
				*startWidth += incrementW;
				offsetX += incrementW;
			}
		}
		
		rect->w = *startWidth;
		rect->h = *startHeight;
		rect->x = *x - rect->w/2;
		rect->y = *y - rect->h/2;
		SDL_BlitScaled(stretch, &texture->getRect(), screenSurface, rect);

		if (lastTickTimeInSeconds >= (timeToStart + timeToEnd))
		{
			complete = true;
			std::cout << "Animation complete" << std::endl;
		}
	}
}

bool Growing::completed()
{
	return false;
}
