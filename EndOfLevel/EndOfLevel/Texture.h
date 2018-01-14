#pragma once

#include <string>
#include <SDL.h>
#include <SDL_image.h>


class Texture
{
public:
	Texture();
	~Texture();

	bool loadFromFile(std::string path, SDL_Renderer *renderer);

	int getWidth();
	int getHeight();

private:
	SDL_Texture* m_texture;

	// image dimensions
	int m_width;
	int m_height;
};