#include "Game.h"
//#include "InputHandler.h"
#include <iostream>

using namespace std;



void Game::Initialize()
{
	isRunning = true; // used for while loop in main
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	//texture = IMG_LoadTexture(renderer, "Assets/spriteSheet.png");
	
	rect.x = 0;
	rect.y = 0;
	rect.h = 600;
	rect.w = 600;

	myTexture.loadFromFile("texture.png", renderer);
	SDL_RenderCopy(renderer, myTexture.getTexture(), &rect, &rect);
	SDL_RenderPresent(renderer);
}