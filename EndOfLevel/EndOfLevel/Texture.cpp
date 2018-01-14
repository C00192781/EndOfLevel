#include "Texture.h"

Texture::Texture()
{
	m_texture = NULL;
	m_height = 0;
	m_width = 0;
}

Texture::~Texture()
{
}

bool Texture::loadFromFile(std::string path, SDL_Renderer *renderer)
{
	SDL_Texture* newTexture = NULL;

	// Load image from the specified path passed in parameters
	SDL_Surface* surface = IMG_Load(path.c_str());
	if (surface != NULL)
	{
		// Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(renderer, surface);
		if (newTexture != NULL)
		{
			// Get image dimensions
			m_width = surface->w;
			m_height = surface->h;
		}

		SDL_FreeSurface(surface);
	}

	m_texture = newTexture;
	return m_texture;
}

SDL_Texture* Texture::getTexture()
{
	return m_texture;
}

int Texture::getWidth()
{
	return m_width;
}

int Texture::getHeight()
{
	return m_height;
}
