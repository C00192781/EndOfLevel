#pragma once
#define SDL_MAIN_HANDLED
#ifdef __APPLE__
#include "SDL2/SDL.h"
#elif defined(_WIN64) || defined(_WIN32)
#include "SDL.h"
#endif

#include "Game.h"
#include <iostream>


// lib files


int main()
{
	Game* game = new Game();

	game->Initialize();
	game->Load();

	while (game->IsRunning())
	{
		game->Update();
		game->Render();
	}

	game->CleanUp();

	SDL_Quit();


	return 0;
}