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
	rect.h = 1800;
	rect.w = 2700;

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
				pixelArray = bloom.BrightPass(&myTexture, stretchedSurface, 1);
				pixelArray = bloom.Blur(&myTexture, kernelRadius, totalPixels, stretchedSurface);
			//	pixelArray = bloom.ApplyBloom(&myTexture, stretchedSurface, totalPixels);
				SDL_UpdateTexture(myTexture.getTexture(), &myTexture.getRect(), pixelArray, myTexture.getPitch());

				break;
			}
		}
	}

}

void Game::Render()
{
	static float xPos = 300;
	static float yPos = 300;
	static float startWidth = 50;
	static float startHeight = 50;
	static float endWidth = 300;
	static float endHeight = 300;	

	grow.Animation(&myTexture, screen, stretchedSurface, &stretchedRect, &xPos, &yPos, &startWidth, &startHeight, &endWidth, &endHeight, 3, 3);

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	myTexture.setColour(r, g, b);
	/*SDL_Point center = { 0, 0 };*/
	//SDL_RenderCopy(renderer, myTexture.getTexture(), NULL, &myTexture.getRect());
	SDL_RenderCopyEx(renderer, myTexture.getTexture(), &rect, &stretchedRect, 0, NULL, SDL_FLIP_NONE);
	SDL_RenderPresent(renderer);
}