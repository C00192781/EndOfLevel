#pragma once

#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>


class Texture
{
public:
	Texture();
	~Texture();

	bool loadFromFile(std::string path, SDL_Renderer *renderer);
	SDL_Texture* getTexture();

	int getWidth();
	int getHeight();
	void setColour(Uint8 red, Uint8 green, Uint8 blue);
	void setBlendMode(SDL_BlendMode blend);

private:
	SDL_Texture* m_texture;

	// image dimensions
	int m_width;
	int m_height;

	int r, g, b; 
};