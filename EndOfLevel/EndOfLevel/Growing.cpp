#include "Growing.h"

Growing::Growing()
{
	lastTickTime, delta = 0.0f;
	complete = false;
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
		if (lastTickTimeInSeconds >= timeToStart)
		{
			static float height = *endHeight - *startHeight;
			static float width = *endWidth - *startWidth;
			static float incrementH = (height / (timeToEnd*mul));
			static float incrementW = (width / (timeToEnd*mul));

			if (*startHeight < *endHeight)
			{
				*startHeight += incrementH;
			}
			if (*startWidth < *endWidth)
			{
				*startWidth += incrementW;
			}
		}

		rect->x = *x;
		rect->y = *y;
		rect->w = *startWidth;
		rect->h = *startHeight;
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
