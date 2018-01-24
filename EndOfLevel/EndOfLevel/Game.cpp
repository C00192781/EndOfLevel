#include "Game.h"
#include <iostream>

using namespace std;

void Game::Initialize()
{
	isRunning = true; // used for while loop in main
	// Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	// Create the window
	window = SDL_CreateWindow("End of Level", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN);
	// retrieve window surface for surface
	screen = SDL_GetWindowSurface(window);


	/// <summary>
	/// Renderer doesn't seem to be used for blitting
	/// </summary>
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	
	rect.x = 0;
	rect.y = 0;
	rect.h = 731;
	rect.w = 500;



	surfaceW = 50;
	surfaceH = 1;
	surfaceX = 250;
	surfaceY = 111;

	kernelRadius = 1;
}


void Game::Load()
{
	myTexture.loadFromFile("ASSETS/texture.png", renderer);

	SDL_Surface* optimizedSurface = NULL;
	SDL_Surface* loadedSurface = IMG_Load("ASSETS/texture.png");

	optimizedSurface = SDL_ConvertSurface(loadedSurface, screen->format, NULL);       
	// No longer need loadedSurface so we git rid of it
	SDL_FreeSurface(loadedSurface);

	stretchedSurface = optimizedSurface;
}


void Game::Render()
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_r:
				r += 8;
				break;

			case SDLK_t:
				r -= 8;
				break;

			case SDLK_g:
				g += 8;
				break;

			case SDLK_h:
				g -= 8;
				break;

			case SDLK_b:
				g += 8;
				break;

			case SDLK_n:
				b -= 8;
				break;

			case SDLK_o:
				kernelRadius++;
				cout << kernelRadius << endl;
				break;

			// Testing
			case SDLK_l:

				kernelSize = kernelRadius * 2 + 1;

				Uint32 *pixelArray = nullptr;
				int totalPixels = myTexture.getTotalPixels();
				pixelArray = new Uint32[totalPixels];
				pixelArray = bloom.Blur(&myTexture, kernelRadius, totalPixels, stretchedSurface);
				SDL_UpdateTexture(myTexture.getTexture(), &myTexture.getRect(), pixelArray, myTexture.getPitch());

				break;
			}
		}
	}

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	myTexture.setColour(r, g, b);

	//// *****************     
	SDL_RenderCopy(renderer, myTexture.getTexture(), &rect, &rect);
	SDL_RenderPresent(renderer);
}
	



	//SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);                       
	//SDL_RenderClear(renderer);                                                       

	//																					                     
	//myTexture.setColour(200, 150, 150);    
	//
	//
	//////myTexture.render(0, 0);       
	//SDL_RenderCopy(renderer, myTexture.getTexture(), &rect, &rect);															
	//SDL_RenderPresent(renderer);




	//if (surfaceW <= 240)
	//{
	//	surfaceW += 1;
	//}
	//if (surfaceH <= 350)
	//{
	//	surfaceH += 0.8;
	//}
	//if (surfaceW <= 240 && surfaceH <= 350)
	//{
	//	surfaceX -= 0.5;
	//	surfaceY -= 0.4;
	//}
	//if (surfaceH <= 350)
	//{
	//	//Apply the image stretched
	//	SDL_Rect stretchedRect;
	//	stretchedRect.x = surfaceX;
	//	stretchedRect.y = surfaceY;
	//	stretchedRect.w = surfaceW;
	//	stretchedRect.h = surfaceH;
	//	SDL_BlitScaled(stretchedSurface, NULL, screen, &stretchedRect);

	//	//if ()
	//	//cout << stretchedSurface->h << endl;
	//	SDL_UpdateWindowSurface(window);
	//}

	

