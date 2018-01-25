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



	surfaceW = 0;
	surfaceH = 0;
	surfaceX = 0;
	surfaceY = 0;

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


void Game::Update()
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
				//pixelArray = bloom.BrightPass(&myTexture, stretchedSurface);
				pixelArray = bloom.Blur(&myTexture, kernelRadius, totalPixels, stretchedSurface);
				//pixelArray = bloom.BloomEffect(&myTexture, stretchedSurface, totalPixels);
				SDL_UpdateTexture(myTexture.getTexture(), &myTexture.getRect(), pixelArray, myTexture.getPitch());

				break;
			}
		}
	}

}

void Game::Render()
{
	


	//if (surfaceW <= 240)
	//{
	//	surfaceW += 1;
	//}
	//if (surfaceH <= 731)
	//{
	//	surfaceH += 0.1;
	//}
	//if (surfaceW <= 240 && surfaceH <= 350)
	//{
	//	surfaceX -= 0.5;
	//	surfaceY -= 0.4;
	//}
	//if (surfaceH <= 350)
	/*{*/
		//Apply the image stretched
		
		//stretchedRect.x = surfaceX;
		//stretchedRect.y = surfaceY;
		//stretchedRect.w = surfaceW;
		//stretchedRect.h = surfaceH;
		//SDL_BlitScaled(stretchedSurface, &myTexture.getRect(), screen, &stretchedRect);

	

	grow.Animation(&myTexture, screen, stretchedSurface, &stretchedRect, 0, 0, 500, 0, myTexture.getWidth(), myTexture.getHeight(), 100, 100, 100);
	/*)*/
	//	////if ()
	//	////cout << stretchedSurface->h << endl;
		//SDL_UpdateWindowSurface(window);
	//}

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	myTexture.setColour(r, g, b);

	const SDL_Point center =  { stretchedRect.w / 2 + stretchedRect.x, stretchedRect.h / 2 + stretchedRect.y };
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	//// *****************  
	//SDL_RenderCopy(renderer, myTexture.getTexture(), &rect, &stretchedRect);
	SDL_RenderCopyEx(renderer, myTexture.getTexture(), &rect, &stretchedRect, 0, NULL, flip);
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






	

